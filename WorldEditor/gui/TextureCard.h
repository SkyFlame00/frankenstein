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
	TextureCard(TextureBrowser::Node* node, QWidget* parent = nullptr);
	~TextureCard();

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
