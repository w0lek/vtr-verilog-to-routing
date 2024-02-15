#include "taskresolver.h"

#include "commconstants.h"
#include "globals.h"
#include "pathhelper.h"
#include "telegramoptions.h"
#include "telegramparser.h"
#include "gtkcomboboxhelper.h"

#include <ezgl/application.hpp>

namespace server {

void TaskResolver::addTask(TaskPtr& newTask)
{
    // pre-process task before adding, where we could quickly detect failure scenarios
    for (const auto& task: m_tasks) {
        if (task->cmd() == newTask->cmd()) {
            if (task->optionsMatch(newTask)) {
                std::string msg = "similar task is already in execution, reject new " + task->info()+ " and waiting for old " + newTask->info() + " execution";
                newTask->fail(msg);
            } else {
                // handle case when task has same jobId but different options
                if (newTask->jobId() > task->jobId()) {
                    std::string msg = "old " + task->info() + " is overriden by a new " + newTask->info();
                    task->fail(msg);
                }
            }
        }
    }

    // add task
    m_tasks.push_back(std::move(newTask));
}

void TaskResolver::addTasks(std::vector<TaskPtr>& tasks)
{
    for (TaskPtr& task: tasks) {
        addTask(task);
    }
}

void TaskResolver::takeFinished(std::vector<TaskPtr>& result)
{
    for (auto it=m_tasks.begin(); it != m_tasks.end();) {
        TaskPtr& task = *it;
        if (task->isFinished()) {
            result.push_back(std::move(task));
            it = m_tasks.erase(it);
        } else {
            ++it;
        }
    }
}

e_timing_report_detail TaskResolver::getDetailsLevelEnum(const std::string& pathDetailsLevelStr) const {
    e_timing_report_detail detailesLevel = e_timing_report_detail::NETLIST;
    if (pathDetailsLevelStr == "netlist") {
        detailesLevel = e_timing_report_detail::NETLIST;
    } else if (pathDetailsLevelStr == "aggregated") {
        detailesLevel = e_timing_report_detail::AGGREGATED;
    } else if (pathDetailsLevelStr == "detailed") {
        detailesLevel = e_timing_report_detail::DETAILED_ROUTING;
    } else if (pathDetailsLevelStr == "debug") {
        detailesLevel = e_timing_report_detail::DEBUG;
    } else {
        std::cerr << "unhandled option" << pathDetailsLevelStr << std::endl;
    }
    return detailesLevel;
}

bool TaskResolver::update(ezgl::application* app)
{
    bool has_processed_task = false;
    for (auto& task: m_tasks) {
        if (!task->isFinished()) {
            switch(task->cmd()) {
                case comm::CMD_GET_PATH_LIST_ID: {
                    processGetPathListTask(app, task);
                    has_processed_task = true;
                    break;
                } 
                case comm::CMD_DRAW_PATH_ID: {
                    processDrawCriticalPathTask(app, task);
                    has_processed_task = true;
                    break;
                }
                default: break;
            }           
        }
    }

    return has_processed_task;
}

void TaskResolver::processGetPathListTask(ezgl::application*, const TaskPtr& task)
{
    TelegramOptions options{task->options(), {comm::OPTION_PATH_NUM, comm::OPTION_PATH_TYPE, comm::OPTION_DETAILS_LEVEL, comm::OPTION_IS_FLOAT_ROUTING}};
    if (!options.hasErrors()) {
        ServerContext& server_ctx = g_vpr_ctx.mutable_server(); // shortcut

        server_ctx.set_crit_path_elements(std::map<std::size_t, std::set<std::size_t>>{}); // reset selection if path list options has changed

        // read options
        const int nCriticalPathNum = options.getInt(comm::OPTION_PATH_NUM, 1);
        const std::string pathType = options.getString(comm::OPTION_PATH_TYPE);
        const std::string detailsLevel = options.getString(comm::OPTION_DETAILS_LEVEL);
        const bool isFlat = options.getBool(comm::OPTION_IS_FLOAT_ROUTING, false);

        // calculate critical path depending on options and store result in server context
        CritPathsResult crit_paths_result = calcCriticalPath(pathType, nCriticalPathNum, getDetailsLevelEnum(detailsLevel), isFlat, /*usePathElementSeparator*/true);

        // setup context
        server_ctx.set_path_type(pathType);
        server_ctx.set_critical_path_num(nCriticalPathNum);
        server_ctx.set_crit_paths(crit_paths_result.paths);

        if (crit_paths_result.isValid()) {
            std::string msg{crit_paths_result.report};
            task->success(msg);
        } else {
            std::string msg{"Critical paths report is empty"};
            std::cerr << msg << std::endl;
            task->fail(msg);
        }
    } else {
        std::string msg{"options errors in get crit path list telegram: " + options.errorsStr()};
        std::cerr << msg << std::endl;
        task->fail(msg);
    }
}

void TaskResolver::processDrawCriticalPathTask(ezgl::application* app, const TaskPtr& task)
{
    TelegramOptions options{task->options(), {comm::OPTION_PATH_ELEMENTS, comm::OPTION_HIGHTLIGHT_MODE, comm::OPTION_DRAW_PATH_CONTOUR}};
    if (!options.hasErrors()) {
        ServerContext& server_ctx = g_vpr_ctx.mutable_server(); // shortcut

        const std::map<std::size_t, std::set<std::size_t>> path_elements = options.getMapOfSets(comm::OPTION_PATH_ELEMENTS);
        const std::string highLightMode = options.getString(comm::OPTION_HIGHTLIGHT_MODE);
        const bool drawPathContour = options.getBool(comm::OPTION_DRAW_PATH_CONTOUR, false);

        // set critical path elements to render
        server_ctx.set_crit_path_elements(path_elements);
        server_ctx.set_draw_crit_path_contour(drawPathContour);

        // update gtk UI
        GtkComboBox* toggle_crit_path = GTK_COMBO_BOX(app->get_object("ToggleCritPath"));
        gint highLightModeIndex = get_item_index_by_text(toggle_crit_path, highLightMode.c_str());
        if (highLightModeIndex != -1) {
            gtk_combo_box_set_active(toggle_crit_path, highLightModeIndex);
            task->success();
        } else {
            std::string msg{"cannot find ToggleCritPath qcombobox index for item " + highLightMode};
            std::cerr << msg << std::endl;
            task->fail(msg);
        }
    } else {
        std::string msg{"options errors in highlight crit path telegram: " + options.errorsStr()};
        std::cerr << msg << std::endl;
        task->fail(msg);
    }
}

} // namespace server
