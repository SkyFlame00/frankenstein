#include "TexturesSettings.h"
#include "../common/GlobalData.h"
#include "TexturePickModal.h"

TexturesSettings::TexturesSettings(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	setLayout(mainLayout);

	/* Path to textures */
	QLabel* pathToTexturesLabel = new QLabel("Path to textures:", this);
	pathToTexturesLabel->setWordWrap(true);
	mainLayout->addWidget(pathToTexturesLabel);
	m_pathToTextures = new QLineEdit(this);
	m_pathToTextures->setText(GlobalData::texturesPath);
	mainLayout->addWidget(m_pathToTextures);
	QLabel* pathToTexturesTip = new QLabel("Be careful, the algorithm will go recursively through all the directories on the specified path", this);
	pathToTexturesTip->setWordWrap(true);
	pathToTexturesTip->setStyleSheet("font-style: italic; font-size: 10px;");
	mainLayout->addWidget(pathToTexturesTip);
	connect(m_pathToTextures, &QLineEdit::editingFinished, this, &TexturesSettings::handlePathToTexturesEditingFinished);

	/* Make previous widgets sit on the top */
	mainLayout->addStretch();
}

void TexturesSettings::handlePathToTexturesEditingFinished()
{
	/* On apply */
	struct ApplyData
	{
		std::string texturesPath;
	};

	ApplyData* onApplyData = new ApplyData;
	onApplyData->texturesPath = m_pathToTextures->text().toStdString();

	auto onApply = [](void* rawData)
	{
		auto data = static_cast<ApplyData*>(rawData);
		GlobalData::texturesPath = data->texturesPath.c_str();
		auto modal = TexturePickModal::getInstance();
		if (modal)
		{
			modal->updateFilesystemTree();
		}
	};

	/* On cancel */
	struct CancelData
	{
		QLineEdit* pathToTextures;
	};
	CancelData* onCancelData = new CancelData{ m_pathToTextures };

	auto onCancel = [](void* rawData)
	{
		auto data = static_cast<CancelData*>(rawData);
		data->pathToTextures->setText(GlobalData::texturesPath);
	};

	emit addApplyingHandler(m_pathToTextures, onApply, onApplyData, onCancel, onCancelData);
}
