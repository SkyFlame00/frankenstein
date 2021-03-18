#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <set>
#include "../common/types.h"
#include "../common/actions.h"

class TextureToolDialog : public QDialog
{
	Q_OBJECT

public:
	TextureToolDialog(QWidget* parent, Qt::WindowFlags flags);
	void onPickedPolygonsChange(std::unordered_map<Types::Polygon*, Brush*>& pickedPolygons);

private:
	void handleShiftXChange(int val);
	void handleShiftXEditingFinished();
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void disableControls();

	int intMax = std::numeric_limits<int>::max();
	int intMin = -(intMax - 1);

	/* Layout */
	QVBoxLayout* m_mainLayout;
	QHBoxLayout* m_texturePreviewLayout;
	QGridLayout* m_textureParamsLayout;

	/* Preview */
	QPushButton* m_changeTextureButton;

	/* Shift X */
	QSpinBox* m_shiftXControl;
	QLabel* m_shiftXLabel;
	bool m_shiftX_editingStarted = false;
	Actions::TextureShiftData* m_textureShiftXData;
	bool m_shiftX_isUndefined = false;
	int m_shiftX_prevVal;

	/* Shift Y */
	QSpinBox* m_shiftYControl;
	QLabel* m_shiftYLabel;

	/* Scale X */
	QSpinBox* m_scaleXControl;
	QLabel* m_scaleXLabel;

	/* Scale Y */
	QSpinBox* m_scaleYControl;
	QLabel* m_scaleYLabel;
};
