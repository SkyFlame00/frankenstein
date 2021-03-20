#pragma once

#include <QDialog>
#include <QString>
#include "../editor/Texture.h"
#include "../editor/ResourceManager.h"
#include "TextureCard.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "FlowLayout.h"
#include <QScrollArea>
#include "../common/types.h"

class TexturePickModal : public QDialog
{
	Q_OBJECT

public:
	TexturePickModal(QWidget* parent = nullptr);
	~TexturePickModal();
	void init();

signals:
	void submit(Texture texture);
	void cancel();

private:
	void makeFilesystemTree(TextureBrowser::Node* parent);
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void closeEvent(QCloseEvent* e) override;
	void handleCardClicked(TextureCard* card);
	void handleCardDoubleClicked(TextureBrowser::Node* node);
	void displayContents(TextureBrowser::Node* dir);
	void clearContents();

	TextureBrowser::Node m_textureRootDir;
	TextureBrowser::Node* m_currentDir;
	QPushButton* m_okButton;
	FlowLayout* m_texturesWindowLayout;
	TextureCard* m_clickedCard = nullptr;
	QList<TextureCard*> m_textureCards;
	Texture m_folderIconTexture;
};
