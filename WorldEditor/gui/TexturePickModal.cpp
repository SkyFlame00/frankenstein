#include "TexturePickModal.h"
#include <filesystem>
#include "../common/GlobalData.h"
#include <QDir>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include "../common/GlobalData.h"

namespace fs = std::filesystem;

TexturePickModal* TexturePickModal::m_instance = nullptr;

TexturePickModal::TexturePickModal(QWidget* parent)
	: QDialog(parent)
{
	setWindowTitle("Texture browser");
	setMinimumSize(800, 640);

	/* Main layout */
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	setLayout(mainLayout);

	/* Texture viewing window */
	QScrollArea* texturesWindow = new QScrollArea(this);
	texturesWindow->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	texturesWindow->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
	texturesWindow->setWidgetResizable(true);

	QWidget* textureCardsContainer = new QWidget(texturesWindow);
	m_texturesWindowLayout = new FlowLayout(textureCardsContainer);
	textureCardsContainer->setLayout(m_texturesWindowLayout);
	texturesWindow->setWidget(textureCardsContainer);
	mainLayout->addWidget(texturesWindow);

	/* Bottom panel */
	QWidget* bottomPanel = new QWidget(this);
	QBoxLayout* bottomPanelLayout = new QBoxLayout(QBoxLayout::RightToLeft, bottomPanel);
	bottomPanel->setLayout(bottomPanelLayout);
	m_okButton = new QPushButton("OK", bottomPanel);
	connect(m_okButton, &QPushButton::clicked, this, &TexturePickModal::handleOkButtonClicked);
	m_cancelButton = new QPushButton("Cancel", bottomPanel);
	connect(m_cancelButton, &QPushButton::clicked, this, &TexturePickModal::handleCancelButtonClicked);
	bottomPanelLayout->addWidget(m_cancelButton);
	bottomPanelLayout->addWidget(m_okButton);
	bottomPanelLayout->addStretch();
	bottomPanelLayout->setMargin(0);
	mainLayout->addWidget(bottomPanel);

	m_textureRootDir = new TextureBrowser::Node;
	m_currentDir = m_textureRootDir;

	/* Folder icon texture */
	QString folderIconPath = QDir::currentPath() + "/assets/icons/folder.png";
	m_folderIconTexture = ResourceManager::getTexture(folderIconPath, true);
}

TexturePickModal::~TexturePickModal()
{
	if (m_textureRootDir)
	{
		deleteFilesystemTree(m_textureRootDir);
	}
}

void TexturePickModal::init()
{
	if (!m_instance)
	{
		m_instance = this;
	}

	makeFilesystemTreeFromRoot();
}

void TexturePickModal::makeFilesystemTreeFromRoot()
{
	m_textureRootDir->type = TextureBrowser::NodeType::DIRECTORY;
	m_textureRootDir->path = GlobalData::texturesPath;
	m_textureRootDir->name = "/";
	m_textureRootDir->texture = m_folderIconTexture;
	makeFilesystemTree(m_textureRootDir);
}

void TexturePickModal::updateFilesystemTree()
{
	deleteFilesystemTree(m_textureRootDir);
	m_textureRootDir = new TextureBrowser::Node;
	m_currentDir = m_textureRootDir;
	makeFilesystemTreeFromRoot();
}

void TexturePickModal::makeFilesystemTree(TextureBrowser::Node* parent)
{
	using namespace TextureBrowser;

	if (!QFile::exists(parent->path))
	{
		return;
	}

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

void TexturePickModal::deleteFilesystemTree(TextureBrowser::Node* node)
{
	for (auto* childNode : node->children)
	{
		deleteFilesystemTree(childNode);
	}

	delete node;
}

void TexturePickModal::showEvent(QShowEvent* event)
{
	m_okButton->setDisabled(true);
	m_currentDir = m_textureRootDir;
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
	m_okButton->setDisabled(card->isDirectory);
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
		handleSubmit(node);
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

void TexturePickModal::handleSubmit(TextureBrowser::Node* node)
{
	emit submit(node->texture);
	hide();
}

void TexturePickModal::handleOkButtonClicked()
{
	handleSubmit(m_clickedCard->getNode());
}

void TexturePickModal::handleCancelButtonClicked()
{
	hide();
}
