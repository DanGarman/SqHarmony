#include "Score.h"

#include "plugin.hpp"
#include "Harmony.h"
#include "WidgetComposite.h"
#include "Harmony1Module.h"
#include "PopupMenuParamWidget.h"

#include "SqMenuItem.h"

struct Harmony1Widget : ModuleWidget {
    Harmony1Widget(Harmony1Module* module) {
        setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/blank-panel-4.svg")));

        addLabel(Vec(28, 5), "Harmony");
        addInputL(Vec(50, 260), Comp::CV_INPUT, "Root");

        const float vlx = 12;
        const float vdelta = 30;
        const float vy = 320;

        addOutputL(Vec(vlx + 0 * vdelta, vy), Comp::BASS_OUTPUT, "B");
        addOutputL(Vec(vlx + 1 * vdelta, vy), Comp::TENOR_OUTPUT, "T");
        addOutputL(Vec(vlx + 2 * vdelta, vy), Comp::ALTO_OUTPUT, "A");
        addOutputL(Vec(vlx + 3 * vdelta, vy), Comp::SOPRANO_OUTPUT, "S");
        addScore(module);

        const float yScale = 150;
        const float yMode = yScale + 30;
        const float xx = 38;

        std::vector<std::string> labels;
        PopupMenuParamWidget* p = createParam<PopupMenuParamWidget>(
            Vec(xx, yScale),
            module,
            Comp::KEY_PARAM);
        p->box.size.x = 80;  // width
        p->box.size.y = 22;
        p->text = "C";
        addParam(p);

        p = createParam<PopupMenuParamWidget>(
            Vec(xx, yMode),
            module,
            Comp::MODE_PARAM);
        p->box.size.x = 80;  // width
        p->box.size.y = 22;
        p->text = "Maj";
        addParam(p);

        const float ySwitch = 210;
        addParam(createParam<CKSSThree>(Vec(xx+50, ySwitch), module, Comp::INVERSION_PREFERENCE_PARAM));
        addLabel(Vec(xx - 30, ySwitch), "Inv. Pref");
    }

    void appendContextMenu(Menu* theMenu) override {
        MenuLabel* spacerLabel = new MenuLabel();
        theMenu->addChild(spacerLabel);

        SqMenuItem_BooleanParam2* item = new SqMenuItem_BooleanParam2(module, Comp::SCORE_COLOR_PARAM);
        item->text = "Black notes on white paper";
        theMenu->addChild(item);
    }

    void step() override {
        ModuleWidget::step();
        if (module) {
            bool whiteOnBlack =  APP->engine->getParamValue(module, Comp::SCORE_COLOR_PARAM) < .5;
          //       SCORE_COLOR_PARAM,  // 0 is white notes, 1 is black notes
            _score->setWhiteOnBlack(whiteOnBlack);
        }
    }

    void addOutputL(const Vec& vec, int outputNumber, const std::string& text) {
        addOutput(createOutput<PJ301MPort>(vec, module, outputNumber));
        Vec vlabel(vec.x, vec.y);
        vlabel.y -= 20;
        const float xOffset = -4 + text.size() * 2.5;            // crude attempt to center text.
        vlabel.x -= xOffset;
        addLabel(vlabel, text);
    }

    void addInputL(const Vec& vec, int outputNumber, const std::string& text) {
        //assert(module);
        addInput(createInput<PJ301MPort>(vec, module, outputNumber));
        Vec vlabel(vec.x, vec.y);
        vlabel.y -= 20;
        const float xOffset = text.size() * 2.5;            // crude attempt to center text.
        vlabel.x -= xOffset;
        addLabel(vlabel, text);
    }

    Label* addLabel(const Vec& v, const std::string& str) {
       // NVGcolor black = nvgRGB(0, 0, 0);
         NVGcolor white = nvgRGB(0xe0, 0xe0, 0xe0);
        Label* label = new Label();
        label->box.pos = v;
        label->text = str;
        label->color = white;
        addChild(label);
        return label;
    }
    void addScore(Harmony1Module* module);
    Score* _score = nullptr;
};


void Harmony1Widget::addScore(Harmony1Module *module) {
    _score = new Score(module);
    auto size = Vec(120, 100);
    auto vu = new BufferingParent(_score, size, _score);


    // 9 too far right
    vu->box.pos = Vec(7, 36),
    INFO("create bp, set width to %f", vu->box.size.x);
    addChild(vu);
}

Model* modelHarmony1 = createModel<Harmony1Module, Harmony1Widget>("sqh-harmony1");