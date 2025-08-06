#include "TitleScene.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "Input.h"
#include "SceneManager.h"
#include "imgui/imgui.h"
#include "MathUtility.h"

#include "TextCommon.h"

using namespace MathUtility;

void TitleScene::Initialize()
{
	TextureManager::GetInstance()->LoadTexture("title.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(SpriteCommon::GetInstance(),"title.png");

	text_ = std::make_unique<Text>();
	text_->Initialize(U"<time=1.0>タ<time=0.1>イム\nいけるかな<time=0.5>...", {640.0f,360.0f}, 400);
	text_->SetTextFormat(Text::TextFormat::Left);
	text_->SetAnchorPoint({0.0f, 0.0f});
	text_->SetSize({1.0f, 1.0f});

}

void TitleScene::Update()
{
	if (Input::GetInstance()->TriggerMouseButton(1)) {
		SceneManager::GetInstance()->SetNextScene("GameScene");
	}
	iii = Input::GetInstance()->GetMouseWheel();
	mouseMove = Input::GetInstance()->GetMousePos();


	float h = TextCommon::GetInstance()->GetFontLoader()->GetHeight();
	float h2 = TextCommon::GetInstance()->GetFontLoader()->GetAscender() - TextCommon::GetInstance()->GetFontLoader()->GetDescender();
	Vector2 sss = text_->GetAnchorPoint();
	Vector2 ddd = text_->GetSize();
	ImGui::Begin("debug");

	ImGui::Text("MouseWheel : %f", iii);
	ImGui::Text("MouseMove : %f,%f", mouseMove.x, mouseMove.y);
	ImGui::Text("H : %f", h);
	ImGui::Text("H2 : %f", h2);
	ImGui::SliderFloat2("textA", &sss.x, 0.0f, 1.0f);
	ImGui::SliderFloat("textB", &ddd.x, 0.0f, 10.0f);
	ImGui::End();
	ddd.y = ddd.x;
	text_->SetAnchorPoint(sss);
	text_->SetSize(ddd);

	sprite_->Updata();

	text_->Update();
}

void TitleScene::Draw()
{
	SpriteCommon::GetInstance()->PreDraw();
//	sprite_->Draw();
	TextCommon::GetInstance()->PreDraw();

	text_->Draw();

	TextCommon::GetInstance()->PostDraw();
}
