#include "TexturePickModal.h"
#include <filesystem>
#include "../common/GlobalData.h"
#include <QDir>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include "../common/GlobalData.h"

namespace fs = std::filesystem;

TexturePickModal::TexturePickModal(QWidget* parent)
	: QDialog(parent)
{
	setMinimumSize(800, 640);

	/* Main layout */
	QVBoxLayout* mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	/* Texture viewing window */
	QScrollArea* texturesWindow = new QScrollArea;
	texturesWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	texturesWindow->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	texturesWindow->setWidgetResizable(true);

	QWidget* textureCardsContainer = new QWidget;
	m_texturesWindowLayout = new FlowLayout;
	textureCardsContainer->setLayout(m_texturesWindowLayout);
	texturesWindow->setWidget(textureCardsContainer);
	mainLayout->addWidget(texturesWindow);

	/* Bottom panel */
	m_okButton = new QPushButton("OK");
	mainLayout->addWidget(m_okButton);
}

TexturePickModal::~TexturePickModal()
{
	/* DELETE TREE */
}

void TexturePickModal::init()
{
	m_textureRootDir.type = TextureBrowser::NodeType::DIRECTORY;
	m_textureRootDir.path = QDir::currentPath() + "/" + GlobalData::texturesPath;
	m_textureRootDir.name = "/";
	makeFilesystemTree(&m_textureRootDir);
}

void TexturePickModal::makeFilesystemTree(TextureBrowser::Node* parent)
{
	using namespace TextureBrowser;

	for (const auto& entry : fs::directory_iterator(parent->path.toStdString()))
	{
		auto pathObj = entry.path();
		auto path = pathObj.string();
		auto name = pathObj.filename().string();
		auto ext = pathObj.extension().string();

		if (entry.is_directory())
		{
			Node* node = new Node{ NodeType::DIRECTORY, path.c_str(), name.c_str(), parent };
			makeFilesystemTree(node);
			parent->children.append(node);
		}
		else if (ext == ".png" || ext == ".jpeg" || ext == ".jpg")
		{
			auto texture = ResourceManager::getTexture(path.c_str(), true);
			Node* node = new Node{ NodeType::TEXTURE, path.c_str(), name.c_str(), parent, texture };
			parent->children.append(node);
		}
	}
}

void TexturePickModal::showEvent(QShowEvent* event)
{
	for (auto* node : m_textureRootDir.children)
	{
		TextureCard* textureCard = new TextureCard(node);
		m_texturesWindowLayout->addWidget(textureCard);
		connect(textureCard, &TextureCard::clicked, this, &TexturePickModal::handleCardClicked);
		connect(textureCard, &TextureCard::doubleClicked, this, &TexturePickModal::handleCardDoubleClicked);
		m_textureCards.append(textureCard);
	}
}

void TexturePickModal::hideEvent(QHideEvent* event)
{
	for (auto* card : m_textureCards)
	{
		disconnect(card, &TextureCard::clicked, this, &TexturePickModal::handleCardClicked);
		disconnect(card, &TextureCard::doubleClicked, this, &TexturePickModal::handleCardDoubleClicked);
		delete card;
	}

	m_clickedCard = nullptr;
	m_textureCards.clear();
}

void TexturePickModal::handleCardClicked(TextureCard* card)
{
	if (m_clickedCard)
	{
		m_clickedCard->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
	}

	card->setStyleSheet("background-color: rgba(89, 143, 255, 1);");
	m_clickedCard = card;
}

void TexturePickModal::handleCardDoubleClicked(TextureBrowser::Node* node)
{
	emit submit(node->texture);
	hide();
}

void TexturePickModal::closeEvent(QCloseEvent* e)
{
	emit cancel();
}
