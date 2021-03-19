#pragma once

#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
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
	void handleShiftYChange(int val);
	void handleShiftYEditingFinished();
	void handleScaleXChange(double val);
	void handleScaleXEditingFinished();
	void handleScaleYChange(double val);
	void handleScaleYEditingFinished();
	void handleUndefined(QSpinBox* control, int val, bool* isUndefined);
	void handleUndefined(QDoubleSpinBox* control, double val, bool* isUndefined);
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void disableControls();

	int shiftMax = std::numeric_limits<int>::max();
	int shiftMin = -(shiftMax - 1);
	float scaleMax = 20.0f;
	float scaleMin = 0.1f;
	int rotationMax = 180;
	int rotationMin = -181;

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
	
	/* Shift Y */
	QSpinBox* m_shiftYControl;
	QLabel* m_shiftYLabel;
	bool m_shiftY_editingStarted = false;
	Actions::TextureShiftData* m_textureShiftYData;
	bool m_shiftY_isUndefined = false;

	/* Scale X */
	QDoubleSpinBox* m_scaleXControl;
	QLabel* m_scaleXLabel;
	bool m_scaleX_editingStarted = false;
	Actions::TextureScaleData* m_textureScaleXData;
	bool m_scaleX_isUndefined = false;

	/* Scale Y */
	QDoubleSpinBox* m_scaleYControl;
	QLabel* m_scaleYLabel;
	bool m_scaleY_editingStarted = false;
	Actions::TextureScaleData* m_textureScaleYData;
	bool m_scaleY_isUndefined = false;
};
