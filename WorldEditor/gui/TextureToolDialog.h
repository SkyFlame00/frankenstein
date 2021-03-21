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
#include "TexturePickModal.h"

class TextureToolDialog : public QDialog
{
	Q_OBJECT

public:
	TextureToolDialog(QWidget* parent, Qt::WindowFlags flags);
	void onPickedPolygonsChange(std::unordered_map<Types::Polygon*, Brush*>& pickedPolygons);
	void init();

	bool shouldChangeDefaultTexture = false;

private:
	void handleShiftXChange(int val);
	void handleShiftXEditingFinished();
	void handleShiftYChange(int val);
	void handleShiftYEditingFinished();
	void handleScaleXChange(double val);
	void handleScaleXEditingFinished();
	void handleScaleYChange(double val);
	void handleScaleYEditingFinished();
	void handleRotationChange(int val);
	void handleRotationEditingFinished();
	void handleUndefined(QSpinBox* control, int val, bool* isUndefined);
	void handleUndefined(QDoubleSpinBox* control, double val, bool* isUndefined);
	void handleChangeTextureButtonClick();
	void handleTexturePickModalSubmit(Texture texture);
	void handleTexturePickModalCancel();
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void disableControls();
	void changePreviewTexture(Texture texture);

	int shiftMax = std::numeric_limits<int>::max();
	int shiftMin = -(shiftMax - 1);
	float scaleMax = 20.0f;
	float scaleMin = 0.1f;
	int rotationMax = std::numeric_limits<int>::max();
	int rotationMin = -(rotationMax - 1);

	/* Preview */
	const int PREVIEW_SIZE = 100;
	QWidget* m_previewContainer;
	QLabel* m_previewLabel;
	QPushButton* m_changeTextureButton;

	/* Texture picking modal */
	TexturePickModal* m_texturePickModal;

	/* Shift X */
	QSpinBox* m_shiftXControl;
	QLabel* m_shiftXLabel;
	bool m_shiftX_editingStarted = false;
	Actions::TextureShiftData* m_textureShiftXData;
	bool m_shiftX_isUndefined = false;
	
	/* Shift Y */
	QSpinBox* m_shiftYControl;
	bool m_shiftY_editingStarted = false;
	Actions::TextureShiftData* m_textureShiftYData;
	bool m_shiftY_isUndefined = false;

	/* Scale X */
	QDoubleSpinBox* m_scaleXControl;
	bool m_scaleX_editingStarted = false;
	Actions::TextureScaleData* m_textureScaleXData;
	bool m_scaleX_isUndefined = false;

	/* Scale Y */
	QDoubleSpinBox* m_scaleYControl;
	bool m_scaleY_editingStarted = false;
	Actions::TextureScaleData* m_textureScaleYData;
	bool m_scaleY_isUndefined = false;

	/* Rotation */
	QSpinBox* m_rotationControl;
	bool m_rotation_editingStarted = false;
	Actions::TextureRotationData* m_textureRotationData;
	bool m_rotation_isUndefined = false;
};
