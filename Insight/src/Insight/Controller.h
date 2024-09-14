#pragma once

#include "Insight.h"
#include "Ui/Panel.h"

namespace Insight {

class Controller {
public:
    virtual ~Controller()
    {
        ClearPanels();
    }

    virtual void OnAdded() {};
    virtual void OnRemoved() {};
    virtual void OnUpdate(float delta)
    {
        const auto panels = m_Panels;

        for (const auto panel : panels)
        {
            panel->OnUpdate(delta);
        }
    };
    virtual void OnRender() {};
    virtual void OnRenderUi()
    {
        const auto panels = m_Panels;

        for (const auto panel : panels)
        {
            panel->OnRender();
        }
    };

    void AddPanel(Panel* panel)
    {
        m_Panels.push_back(panel);
        panel->OnAdded();
    }

    void RemovePanel(Panel* panel)
    {
        panel->OnRemoved();
        std::erase(m_Panels, panel);
        delete panel;
    }

    void ClearPanels()
    {
        for (const auto panel : m_Panels)
        {
            panel->OnRemoved();
            delete panel;
        }

        m_Panels.clear();
    }

protected:
    string m_Name;

    vector<Panel *> m_Panels;
};

} // Insight
