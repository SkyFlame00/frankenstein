#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QObject>

class TexturesSettings : public QWidget
{
	Q_OBJECT
	
public:
	/* Qt meta-object compiler cannot resolve function pointers as params, so we need a workaround like this one */
	typedef void (*handler_t)(void* data);

	TexturesSettings(QWidget* parent = nullptr);

signals:
	void addApplyingHandler(QObject* object, handler_t onApply, void* onApplyData,
		handler_t onCancel, void* onCancelData);

private:
	void handlePathToTexturesEditingFinished();

	QLineEdit* m_pathToTextures;
};
