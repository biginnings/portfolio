#include "stdafx.h"
#include "LayerIngameUI.h"

LayerIngameUI* LayerIngameUI::create()
{
	auto ret = new LayerIngameUI();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}


bool LayerIngameUI::init()
{
	if (!Node::init()) return false;

	addChild(lbScore = Label::createWithTTF("adsgs", FONT_NAME, 48.0f));
	lbScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	lbScore->setPosition(Vec2(30, 1280 - 180));

	addChild(btnPause = Button::create("res/btn_pause_normal.png", "res/btn_pause_pressed.png", "res/btn_pause_disabled.png"));
	btnPause->setPosition(Vec2(720 - 70, 1280 - 180));

	const Size PANEL_SIZE(600, 400);
	const float SPACING = 170;
	const float BUTTON_BOTTOM_SPACING = 100;

	addChild(dnCurtain = DrawNode::create());
	dnCurtain->drawSolidRect(Vec2::ZERO, Vec2(720, 1280), Color4F(0, 0, 0, 0.8));

	addChild(pausePanel = Scale9Sprite::create("res/panel.png"));	// Scals9Sprite : 이미지가 깨지지 않는 sprite
	pausePanel->setPosition(Vec2(720 / 2, 1280 / 2));
	pausePanel->setScale9Enabled(true);	// 이미지를 9등분으로 해서  일부분만 늘려 사람이 봤을 때 안 늘어난 것처럼 보임
	pausePanel->setContentSize(Size(600, 400));	// 가로 세로 크기를 지정해줄 수 있음

	pausePanel->addChild(btnRestart = Button::create("res/btn_restart_normal.png", "res/btn_restart_pressed.png", "res/btn_restart_disabled.png"));
	pausePanel->addChild(btnHome = Button::create("res/btn_home_normal.png", "res/btn_home_pressed.png", "res/btn_home_disabled.png"));
	pausePanel->addChild(btnResume = Button::create("res/btn_play_normal.png", "res/btn_play_pressed.png", "res/btn_play_disabled.png"));

	btnResume->setPosition(Vec2(PANEL_SIZE.width / 2, BUTTON_BOTTOM_SPACING));
	btnHome->setPosition(Vec2(PANEL_SIZE.width / 2 - SPACING, BUTTON_BOTTOM_SPACING));
	btnRestart->setPosition(Vec2(PANEL_SIZE.width / 2 + SPACING, BUTTON_BOTTOM_SPACING));

	Label* lbPaused = Label::createWithTTF("PAUSED!!!", FONT_NAME, 64.0f);
	pausePanel->addChild(lbPaused);
	lbPaused->setColor(Color3B(0, 0, 0));
	lbPaused->setPosition(Vec2(PANEL_SIZE.width / 2, 300));

	setScore(0);
	hidePausePanel();

	return true;
}

void LayerIngameUI::setScore(long long score)
{
	sprintf(scoreString, "Score: %ld", score);
	lbScore->setString(scoreString);
}

long long LayerIngameUI::getScore()
{
	return score;
}

void LayerIngameUI::showPausePanel()
{
	pausePanel->setVisible(true);
	dnCurtain->setVisible(true);

	dnCurtain->setOpacity(0);
	dnCurtain->runAction(FadeIn::create(0.125));

	auto pos = pausePanel->getPosition();
	pausePanel->setPosition(pos - Vec2(0, 1000));
	pausePanel->runAction(EaseExponentialInOut::create(MoveTo::create(0.125f, pos)));
}

void LayerIngameUI::hidePausePanel()
{

	auto pos = pausePanel->getPosition();
	pausePanel->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(0.25f, pos - Vec2(0, 1000))), CallFunc::create([=]() {
		pausePanel->setPosition(pos);
		pausePanel->setVisible(false);
		dnCurtain->setVisible(false);
	}),
	nullptr));

}
