#pragma once

#include "ui/ui.h"

class Prediction
{
public:
    Prediction(UI *ui);

    void toggle();

private:
    bool m_toggled = false;
    UI *m_ui;
};