#pragma once

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPushButton>
#include "TexturesSettings.h"

class SettingsWindow : public QDialog
{
	Q_OBJECT

public:
	SettingsWindow(QWidget* parent = nullptr);

private:
	struct ApplyingHandler
	{
		void (*onApply)(void*);
		void* onApplyData;
		void (*onCancel)(void*);
		void* onCancelData;
	};

	void handleTitleClicked(QListWidgetItem* item);
	void onAddApplyingHandler(QObject* object, void (*onApply)(void*), void* onApplyData,
		void (*onCancel)(void*), void* onCancelData);
	void apply();
	void cancel();
	void handleOkButtonClick(bool checked);
	void handleCancelButtonClick(bool checked);
	void closeEvent(QCloseEvent* event) override;
	void updateConfigurationFile();

	const int MIN_WIDTH = 650;
	const int MIN_HEIGHT = 420;

	std::unordered_map<QObject*, ApplyingHandler> applyingHandlers;

	QListWidget* m_sectionsTitles;
	QWidget* m_sectionContainer;
	QHBoxLayout* m_sectionContainerLayout;

	/* Bottom panel */
	QPushButton* m_okButton;
	QPushButton* m_cancelButton;

	/* Texture settings */
	QListWidgetItem* m_texturesSettingsItem;
	TexturesSettings* m_texturesSettings;
};
