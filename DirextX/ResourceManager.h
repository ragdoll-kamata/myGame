#pragma once
class ResourceManager {
public:

	void ResetResources() {
		lightCost = 0;
		darknessCost = 0;
		money_ = 0;
		rampageGauge_ = 0.0f;
		score_ = 0;
	}

	int GetLightCost() const {
		return lightCost;
	}
	int GetDarknessCost() const {
		return darknessCost;
	}
	void AddLightCost(int amount) {
		lightCost += amount;
		if (lightCost < 0) {
			lightCost = 0;
		}
	}
	void AddDarknessCost(int amount) {
		darknessCost += amount;
		if (darknessCost < 0) {
			darknessCost = 0;
		}
	}
	int GetMoney() const {
		return money_;
	}
	void AddMoney(int amount) {
		money_ += amount;
		if (money_ < 0) {
			money_ = 0;
		}
	}
	float GetRampageGauge() const {
		return rampageGauge_;
	}
	float GetMaxRampageGauge() const {
		return maxRampageGauge_;
	}
	void AddRunawayGauge(float amount) {
		rampageGauge_ += amount;
		if (rampageGauge_ < 0.0f) {
			rampageGauge_ = 0.0f;
		} else if (rampageGauge_ > maxRampageGauge_) {
			rampageGauge_ = maxRampageGauge_;
		}
	}
	int GetScore() const {
		return score_;
	}
	void AddScore(int amount) {
		score_ += amount;
		if (score_ < 0) {
			score_ = 0;
		}
	}

private:
	// エネルギーコスト
	int lightCost = 0;
	int darknessCost = 0;

	// 所持金
	int money_ = 0;

	// 暴走ゲージ
	float rampageGauge_ = 0;
	// 最大暴走ゲージ
	const float maxRampageGauge_ = 500.0f;

	// スコア
	int score_ = 0;
};

