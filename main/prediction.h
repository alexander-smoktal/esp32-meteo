#pragma once

#include "ui/ui.h"

class Prediction
{
public:
    Prediction(UI *ui);

    void toggle();

private:
    UI *m_ui;
};