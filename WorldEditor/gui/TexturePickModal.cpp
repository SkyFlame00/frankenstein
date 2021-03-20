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

	m_currentDir = &m_textureRootDir;

	/* Folder icon texture */
	QString folderIconPath = QDir::currentPath() + "/assets/icons/folder.png";
	m_folderIconTexture = ResourceManager::getTexture(folderIconPath, true);
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
	m_textureRootDir.texture = m_folderIconTexture;
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
			Node* node = new Node{ NodeType::DIRECTORY, path.c_str(), name.c_str(), parent, m_folderIconTexture };
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
	m_currentDir = &m_textureRootDir;
	displayContents(m_currentDir);
}

void TexturePickModal::hideEvent(QHideEvent* event)
{
	clearContents();
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
	if (node->type == TextureBrowser::NodeType::DIRECTORY)
	{
		m_currentDir = node;
		displayContents(node);
	}
	else if (node->type == TextureBrowser::NodeType::TEXTURE)
	{
		emit submit(node->texture);
		hide();
	}
}

void TexturePickModal::closeEvent(QCloseEvent* e)
{
	emit cancel();
}

void TexturePickModal::displayContents(TextureBrowser::Node* dir)
{
	clearContents();

	auto createCard = [&](TextureBrowser::Node* node, QString labelStr = "")
	{
		TextureCard* textureCard = new TextureCard(node, labelStr);
		m_texturesWindowLayout->addWidget(textureCard);
		connect(textureCard, &TextureCard::clicked, this, &TexturePickModal::handleCardClicked);
		connect(textureCard, &TextureCard::doubleClicked, this, &TexturePickModal::handleCardDoubleClicked);
		m_textureCards.append(textureCard);
	};

	if (dir->parent)
		createCard(dir->parent, "...");

	for (auto* node : dir->children)
		createCard(node);
}

void TexturePickModal::clearContents()
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