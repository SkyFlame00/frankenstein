#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>
#include <QBoxLayout>
#include "../common/types.h"

class TextureCard : public QWidget
{
	Q_OBJECT

public:
	TextureCard(TextureBrowser::Node* node, QString labelStr = "", QWidget* parent = nullptr);
	~TextureCard();
	inline TextureBrowser::Node* getNode() { return m_node; }

	bool isDirectory;

signals:
	void clicked(TextureCard* card);
	void doubleClicked(TextureBrowser::Node* node);

private:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

	const int SIZE = 200;
	QWidget* m_textureContainer;
	TextureBrowser::Node* m_node;
};
