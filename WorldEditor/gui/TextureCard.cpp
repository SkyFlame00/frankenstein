#include "TextureCard.h"

TextureCard::TextureCard(TextureBrowser::Node* node, QString labelStr, QWidget* parent)
	: QWidget(parent)
{
	m_node = node;
	setFixedWidth(SIZE);

	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	setLayout(mainLayout);

	m_textureContainer = new QWidget;
	QGridLayout* textureContainerLayout = new QGridLayout;
	m_textureContainer->setLayout(textureContainerLayout);
	textureContainerLayout->setMargin(0);
	textureContainerLayout->setSpacing(0);

	auto& image = node->texture.image;
	int width = image.width();
	int height = image.height();
	int max = std::max(image.width(), image.height());
	QPixmap pixmap;

	if (max > SIZE)
	{
		pixmap = QPixmap::fromImage(image).scaled({ SIZE, SIZE }, Qt::KeepAspectRatio);
	}
	else
	{
		pixmap = QPixmap::fromImage(image);
	}

	QLabel* imageLabel = new QLabel;
	imageLabel->setPixmap(pixmap);

	int marginLeft = (float)SIZE / 2 - (float)pixmap.width() / 2;
	m_textureContainer->setContentsMargins(marginLeft, 0, 0, 0);
	m_textureContainer->setFixedSize(SIZE, SIZE);
	
	textureContainerLayout->addWidget(imageLabel);
	mainLayout->addWidget(m_textureContainer);

	/* Texture name */
	QLabel* label = new QLabel(labelStr.size() > 0 ? labelStr : node->name);
	label->setContentsMargins(5, 5, 5, 5);
	mainLayout->addWidget(label);
}

TextureCard::~TextureCard()
{
}

void TextureCard::mousePressEvent(QMouseEvent* event)
{
	emit clicked(this);
}

void TextureCard::mouseDoubleClickEvent(QMouseEvent* event)
{
	emit doubleClicked(m_node);
}
