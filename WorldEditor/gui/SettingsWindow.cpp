#include "SettingsWindow.h"
#include <QDebug>
#include "../common/types.h"
#include <QFile>
#include "../common/GlobalData.h"
#include <nlohmann/json.hpp>

using nlohmann::json;

SettingsWindow::SettingsWindow(QWidget* parent)
	: QDialog(parent)
{
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

	/* Container */
	QVBoxLayout* containerLayout = new QVBoxLayout(this);
	setLayout(containerLayout);

	/* Main widget */
	QWidget* mainWidget = new QWidget(this);
	QHBoxLayout* mainLayout = new QHBoxLayout(mainWidget);
	mainLayout->setDirection(QBoxLayout::LeftToRight);
	containerLayout->addWidget(mainWidget);

	/* Bottom panel */
	QWidget* bottomPanel = new QWidget(this);
	containerLayout->addWidget(bottomPanel);
	QHBoxLayout* bottomPanelLayout = new QHBoxLayout(bottomPanel);
	bottomPanel->setLayout(bottomPanelLayout);
	bottomPanelLayout->setMargin(0);
	m_okButton = new QPushButton("OK", bottomPanel);
	connect(m_okButton, &QPushButton::clicked, this, &SettingsWindow::handleOkButtonClick);
	m_cancelButton = new QPushButton("Cancel", bottomPanel);
	connect(m_cancelButton, &QPushButton::clicked, this, &SettingsWindow::handleCancelButtonClick);
	bottomPanelLayout->addStretch();
	bottomPanelLayout->addWidget(m_okButton);
	bottomPanelLayout->addWidget(m_cancelButton);

	/* Sections' titles */
	m_sectionsTitles = new QListWidget(mainWidget);
	m_sectionsTitles->setViewMode(QListView::ListMode);
	m_sectionsTitles->setFixedWidth(200);
	mainLayout->addWidget(m_sectionsTitles);

	/* Section container */
	m_sectionContainer = new QWidget(mainWidget);
	m_sectionContainerLayout = new QHBoxLayout(m_sectionContainer);
	m_sectionContainer->setLayout(m_sectionContainerLayout);
	mainLayout->addWidget(m_sectionContainer);

	/* Sections' titles items */
	m_texturesSettingsItem = new QListWidgetItem("Textures", m_sectionsTitles);
	m_texturesSettings = new TexturesSettings(m_sectionContainer);
	m_sectionContainerLayout->addWidget(m_texturesSettings);
	m_texturesSettings->hide();

	connect(m_sectionsTitles, &QListWidget::itemClicked, this, &SettingsWindow::handleTitleClicked);
	connect(m_texturesSettings, &TexturesSettings::addApplyingHandler, this, &SettingsWindow::onAddApplyingHandler);
}

void SettingsWindow::handleTitleClicked(QListWidgetItem* item)
{
	if (item == m_texturesSettingsItem)
	{
		m_texturesSettings->show();
	}
}

void SettingsWindow::onAddApplyingHandler(QObject* object, void (*onApply)(void*), void* onApplyData,
	void (*onCancel)(void*), void* onCancelData)
{
	applyingHandlers[object] = { onApply, onApplyData, onCancel, onCancelData };
}

void SettingsWindow::apply()
{
	for (auto [_, data] : applyingHandlers)
	{
		data.onApply(data.onApplyData);
		delete data.onApplyData;
	}

	applyingHandlers.clear();
	updateConfigurationFile();
}

void SettingsWindow::cancel()
{
	for (auto [_, data] : applyingHandlers)
	{
		data.onCancel(data.onCancelData);
		delete data.onCancelData;
	}

	applyingHandlers.clear();
}

void SettingsWindow::closeEvent(QCloseEvent* event)
{
	cancel();
}

void SettingsWindow::handleOkButtonClick(bool checked)
{
	apply();
	hide();
}

void SettingsWindow::handleCancelButtonClick(bool checked)
{
	close();
}

void SettingsWindow::updateConfigurationFile()
{
	QFile file(GlobalData::configPath);

	if (!file.open(QIODevice::WriteOnly))
	{
		qWarning() << "Could not open configuration file";
		return;
	}

	QTextStream outStream(&file);
	json output = {
		{"texture_settings", {
			{"texture_path", GlobalData::texturesPath.toStdString()}
		}}
	};

	outStream << output.dump().c_str();
}
