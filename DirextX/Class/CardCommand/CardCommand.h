#pragma once
#include "Card.h"
#include <string>
#include <vector>

enum class ExecuteResult {
	Normal,
	Standby,
	Return,
	Break,
	Continue,
	Error,
};
enum class CommandType {
	Void,
	None,
};

class CardCommand {
public:
	enum class ParseBoolType {
		Element,
		Type,
		Int,
		Name,
		Bool,
		Operators,
	};

	struct ParseBoolData {
		ParseBoolType type;
		std::string value;
		bool reverse = false;

	};
	struct ParseBoolResult;
	struct ParseBoolGroup {
		std::vector<ParseBoolData> dates;
		bool chain; // true: &&, false: ||
		std::unique_ptr<ParseBoolResult> next = nullptr;
	};
	struct ParseBoolResult {
		std::vector<ParseBoolGroup> groups;
	};
	enum class IntExprNodeType {
		Num,
		Add,
		Subtract,
		Multiply,
		Division,
		None,
	};

	struct ExprNode {
		IntExprNodeType type = IntExprNodeType::None;
		std::string str;
		std::unique_ptr<ExprNode> left, right;
	};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CardCommand() = default;

	/// <summary>
	/// カードコマンド実行
	/// </summary>
	/// <param name="card">カード</param>
	/// <returns>リザルト</returns>
	virtual ExecuteResult Execute(Card* card) = 0;

	/// <summary>
	/// コマンドタイプ取得
	/// </summary>
	/// <returns></returns>
	virtual CommandType GetCommandType() {
		return CommandType::None;
	}

	/// <summary>
	/// カードマネージャー設定
	/// </summary>
	/// <param name="cardManager"></param>
	static void SetCardManager(CardManager* cardManager) {
		cardManager_ = cardManager;
	}
protected:
	/// <summary>
	/// 整数のパース
	/// </summary>
	/// <param name="num">パースする文字</param>
	/// <param name="card">カード</param>
	/// <returns>整数</returns>
	int ParseInt(std::string num, Card* card);
	/// <summary>
	/// カードのパース
	/// </summary>
	/// <param name="cardNum">カード変数名</param>
	/// <param name="cards">取得したカード</param>
	/// <param name="card">カード</param>
	/// <returns>成功したかどうか</returns>
	bool ParseCard(const std::string& cardNum, std::vector<Card*>& cards, Card* card);
	/// <summary>
	/// キーに応じたカードのパース
	/// </summary>
	/// <param name="key">キー</param>
	/// <param name="cardNum"></param>
	/// <param name="cards">取得したカード</param>
	/// <param name="card">カード</param>
	/// <returns>成功したかどうか</returns>
	bool ParseCardIfKey(const std::string& key, const std::string& cardNum, std::vector<Card*>& cards, Card* card);
	/// <summary>
	/// カード属性のパース
	/// </summary>
	/// <param name="element">パースする文字</param>
	/// <param name="card">カード</param>
	/// <returns>属性</returns>
	CardElement ParseCardElement(std::string element, Card* card);
	/// <summary>
	/// カードタイプのパース
	/// </summary>
	/// <param name="type">パースする文字</param>
	/// <param name="card">カード</param>
	/// <returns>タイプ</returns>
	CardType ParseCardType(std::string type, Card* card);
	/// <summary>
	/// カード特性のパース
	/// </summary>
	/// <param name="characteristics">パースする文字</param>
	/// <returns特性></returns>
	CardCharacteristics ParseCardCharacteristics(std::string characteristics);
	/// <summary>
	/// カードレアリティのパース
	/// </summary>
	/// <param name="rarity">パースする文字</param>
	/// <returns>レアリティ</returns>
	CardRarity ParseCardRarity(std::string rarity);
	/// <summary>
	/// 建物耐久値減少タイミングのパース
	/// </summary>
	/// <param name="timing">パースする文字</param>
	/// <returns>建物耐久値減少タイミング</returns>
	BuildingDurabilityDecreasenTiming ParseBuildingDurabilityDecreasenTiming(std::string timing);
	/// <summary>
	/// 建物発動タイミングのパース
	/// </summary>
	/// <param name="timing">パースする文字</param>
	/// <returns>建物発動タイミング</returns>
	BuildingActivationTiming ParseBuildingActivationTiming(std::string timing);
	/// <summary>
	/// UTF-8文字列をUTF-32文字列に変換する
	/// </summary>
	/// <param name="str">変換する文字</param>
	/// <returns>UTF-32</returns>
	std::u32string Utf8ToU32(const std::string& str);

	/// <summary>
	/// ブール式のパース
	/// </summary>
	/// <param name="boolTokens">パースするトークン</param>
	/// <returns>boolリザルト</returns>
	std::unique_ptr<ParseBoolResult> ParseBool(const std::vector<std::string>& boolTokens);

	/// <summary>
	/// ブール式の実行
	/// </summary>
	/// <param name="parseBoolResult">boolリザルト</param>
	/// <param name="card">カード</param>
	/// <returns>bool</returns>
	bool ExecuteBool(std::unique_ptr<ParseBoolResult>& parseBoolResult, Card* card);

	/// <summary>
	/// 文字列のトークン分割
	/// </summary>
	/// <param name="str">分割する文字</param>
	/// <param name="token">分割したトークン</param>
	/// <returns>成功したかどうか</returns>
	bool Parse(std::string str, std::vector<std::string>& token);

	/// <summary>
	/// int式ノードの作成
	/// </summary>
	/// <param name="tokens">パースするトークン</param>
	/// <returns>親ノード</returns>
	std::unique_ptr<ExprNode> CreateIntExprNode(std::vector<std::string>& tokens);

	/// <summary>
	/// int式ノードの計算
	/// </summary>
	/// <param name="root">親ノード</param>
	/// <param name="card">カード</param>
	/// <returns>計算した値</returns>
	int CalculationIntExprNode(std::unique_ptr<ExprNode>& root, Card* card);

	/// <summary>
	/// カード式ノードの作成
	/// </summary>
	/// <param name="tokens">パースするトークン</param>
	/// <returns>親ノード</returns>
	std::unique_ptr<ExprNode> CreateCardExprNode(std::vector<std::string>& tokens);

	/// <summary>
	/// カード式ノードの計算
	/// </summary>
	/// <param name="root">親ノード</param>
	/// <param name="cards">出来たカード束</param>
	/// <param name="card">カード</param>
	/// <returns>成功したかどうか</returns>
	bool CalculationCardExprNode(std::unique_ptr<ExprNode>& root, std::vector<Card*>& cards, Card* card);

	bool CalcSelectCardIf(std::vector<Card*>& cards, std::vector<Card*>& trueCards, Card* card);
private:
	/// <summary>
	/// 式ノードの設定
	/// </summary>
	/// <param name="root">親ノード</param>
	/// <param name="node">入れようとしてるノード</param>
	void ExprNodeSet(std::unique_ptr<ExprNode>& root, std::unique_ptr<ExprNode>& node);
	
	

protected:
	static CardManager* cardManager_;
};

