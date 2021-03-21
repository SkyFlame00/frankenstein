#include "TextureToolDialog.h"
#include "../common/GlobalData.h"
#include "../common/helpers.h"
#include <limits>
#include "../common/ActionHistoryTool.h"
#include "MainWindow.h"
#include "../common/ActionHistoryTool.h"
#include "../common/actions.h"
#include <QDir>

TextureToolDialog::TextureToolDialog(QWidget* parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	setWindowTitle("Texture tool");
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

	/* Main layout */
	QHBoxLayout* m_mainLayout = new QHBoxLayout(this);
	
	/* Texture preview and change texture button */
	QWidget* leftSideContainer = new QWidget(this);
	QVBoxLayout* leftSideLayout = new QVBoxLayout(leftSideContainer);
	leftSideContainer->setLayout(leftSideLayout);
	m_mainLayout->addWidget(leftSideContainer);

	m_previewContainer = new QWidget(leftSideContainer);
	m_previewContainer->setStyleSheet("border: 1px solid rgb(100, 100, 100);");
	QGridLayout* previewContainerLayout = new QGridLayout(m_previewContainer);
	m_previewContainer->setLayout(previewContainerLayout);
	previewContainerLayout->setMargin(0);
	previewContainerLayout->setSpacing(0);
	m_previewLabel = new QLabel(m_previewContainer);
	previewContainerLayout->addWidget(m_previewLabel);
	leftSideLayout->addWidget(m_previewContainer);

	m_changeTextureButton = new QPushButton(leftSideContainer);
	m_changeTextureButton->setText("Change texture");
	leftSideLayout->addWidget(m_changeTextureButton);
	connect(m_changeTextureButton, &QAbstractButton::clicked, this, &TextureToolDialog::handleChangeTextureButtonClick);

	/* Texture picking modal */
	m_texturePickModal = new TexturePickModal(this);

	/* Texture parameters */
	QWidget* m_textureParamsContainer = new QWidget;
	QGridLayout* m_textureParamsLayout = new QGridLayout(m_textureParamsContainer);
	m_textureParamsLayout->setSizeConstraint(QLayout::SetFixedSize);
	m_textureParamsContainer->setLayout(m_textureParamsLayout);
	m_mainLayout->addWidget(m_textureParamsContainer);

	/* Shift X */
	QWidget* m_shiftXContainer = new QWidget;
	QHBoxLayout* m_shiftXLayout = new QHBoxLayout;
	QLabel* m_shiftXLabel = new QLabel("Shift X");
	m_shiftXControl = new QSpinBox;
	m_shiftXControl->setSpecialValueText("-");
	m_shiftXControl->setMinimum(shiftMin);
	m_shiftXControl->setMaximum(shiftMax);
	m_shiftXLayout->addWidget(m_shiftXLabel);
	m_shiftXLayout->addWidget(m_shiftXControl);
	m_shiftXContainer->setLayout(m_shiftXLayout);
	connect(m_shiftXControl, qOverload<int>(&QSpinBox::valueChanged), this, &TextureToolDialog::handleShiftXChange);
	connect(m_shiftXControl, &QSpinBox::editingFinished, this, &TextureToolDialog::handleShiftXEditingFinished);

	/* Shift Y */
	QWidget* m_shiftYContainer = new QWidget;
	QHBoxLayout* m_shiftYLayout = new QHBoxLayout;
	QLabel* m_shiftYLabel = new QLabel("Shift Y");
	m_shiftYControl = new QSpinBox;
	m_shiftYControl->setSpecialValueText("-");
	m_shiftYControl->setMinimum(shiftMin);
	m_shiftYControl->setMaximum(shiftMax);
	m_shiftYLayout->addWidget(m_shiftYLabel);
	m_shiftYLayout->addWidget(m_shiftYControl);
	m_shiftYContainer->setLayout(m_shiftYLayout);
	connect(m_shiftYControl, qOverload<int>(&QSpinBox::valueChanged), this, &TextureToolDialog::handleShiftYChange);
	connect(m_shiftYControl, &QSpinBox::editingFinished, this, &TextureToolDialog::handleShiftYEditingFinished);

	/* Scale X */
	QWidget* m_scaleXContainer = new QWidget;
	QHBoxLayout* m_scaleXLayout = new QHBoxLayout;
	QLabel* m_scaleXLabel = new QLabel("Scale X");
	m_scaleXControl = new QDoubleSpinBox;
	m_scaleXControl->setMinimum(scaleMin);
	m_scaleXControl->setMaximum(scaleMax);
	m_scaleXControl->setSpecialValueText("-");
	m_scaleXControl->setSingleStep(0.1f);
	m_scaleXLayout->addWidget(m_scaleXLabel);
	m_scaleXLayout->addWidget(m_scaleXControl);
	m_scaleXContainer->setLayout(m_scaleXLayout);
	connect(m_scaleXControl, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &TextureToolDialog::handleScaleXChange);
	connect(m_scaleXControl, &QDoubleSpinBox::editingFinished, this, &TextureToolDialog::handleScaleXEditingFinished);

	/* Scale Y */
	QWidget* m_scaleYContainer = new QWidget;
	QHBoxLayout* m_scaleYLayout = new QHBoxLayout;
	QLabel* m_scaleYLabel = new QLabel("Scale Y");
	m_scaleYControl = new QDoubleSpinBox;
	m_scaleYControl->setMinimum(scaleMin);
	m_scaleYControl->setMaximum(scaleMax);
	m_scaleYControl->setSpecialValueText("-");
	m_scaleYControl->setSingleStep(0.1f);
	m_scaleYLayout->addWidget(m_scaleYLabel);
	m_scaleYLayout->addWidget(m_scaleYControl);
	m_scaleYContainer->setLayout(m_scaleYLayout);
	connect(m_scaleYControl, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &TextureToolDialog::handleScaleYChange);
	connect(m_scaleYControl, &QDoubleSpinBox::editingFinished, this, &TextureToolDialog::handleScaleYEditingFinished);

	/* Rotation */
	QWidget* m_rotationContainer = new QWidget;
	QHBoxLayout* m_rotationLayout = new QHBoxLayout;
	QLabel* m_rotationLabel = new QLabel("Rotation");
	m_rotationControl = new QSpinBox;
	m_rotationControl->setSpecialValueText("-");
	m_rotationControl->setMinimum(rotationMin);
	m_rotationControl->setMaximum(rotationMax);
	m_rotationLayout->addWidget(m_rotationLabel);
	m_rotationLayout->addWidget(m_rotationControl);
	m_rotationContainer->setLayout(m_rotationLayout);
	connect(m_rotationControl, qOverload<int>(&QSpinBox::valueChanged), this, &TextureToolDialog::handleRotationChange);
	connect(m_rotationControl, &QSpinBox::editingFinished, this, &TextureToolDialog::handleRotationEditingFinished);

	/* Populate with created widgets */
	m_textureParamsLayout->addWidget(m_shiftXContainer, 0, 0, 1, 1);
	m_textureParamsLayout->addWidget(m_shiftYContainer, 0, 1, 1, 1);
	m_textureParamsLayout->addWidget(m_scaleXContainer, 1, 0, 1, 1);
	m_textureParamsLayout->addWidget(m_scaleYContainer, 1, 1, 1, 1);
	m_textureParamsLayout->addWidget(m_rotationContainer, 2, 0, 1, 1);

	setLayout(m_mainLayout);

	connect(m_texturePickModal, &TexturePickModal::submit, this, &TextureToolDialog::handleTexturePickModalSubmit);
	connect(m_texturePickModal, &TexturePickModal::cancel, this, &TextureToolDialog::handleTexturePickModalCancel);
}

void TextureToolDialog::init()
{
	changePreviewTexture(GlobalData::applyingTexture);
	m_texturePickModal->init();
}

void TextureToolDialog::changePreviewTexture(Texture texture)
{
	auto& image = texture.image;
	int width = image.width();
	int height = image.height();
	int max = std::max(image.width(), image.height());
	QPixmap pixmap;

	if (max > PREVIEW_SIZE)
	{
		pixmap = QPixmap::fromImage(image).scaled({ PREVIEW_SIZE, PREVIEW_SIZE }, Qt::KeepAspectRatio);
	}
	else
	{
		pixmap = QPixmap::fromImage(image);
	}

	m_previewLabel->setPixmap(pixmap);

	int marginLeft = (float)PREVIEW_SIZE / 2 - (float)pixmap.width() / 2;
	m_previewContainer->setContentsMargins(marginLeft, 0, 0, 0);
	m_previewContainer->setFixedSize(PREVIEW_SIZE, PREVIEW_SIZE);
}

void TextureToolDialog::showEvent(QShowEvent* event)
{
	auto* global = GlobalData::getInstance();
	auto& sdata = global->m_selectionToolData;
	auto& tdata = global->textureToolData;

	changePreviewTexture(GlobalData::applyingTexture);

	if (sdata.renderable)
	{
		auto* brush = sdata.renderable;
		sdata.renderable->m_selected = false;
		sdata.renderable = nullptr;
		this->shouldChangeDefaultTexture = true;

		for (auto* polygon : brush->getPolygons())
		{
			tdata.pickedPolygons[polygon] = brush;
			brush->selectPolygon(polygon);
		}

		onPickedPolygonsChange(tdata.pickedPolygons);
	}
	else
	{
		disableControls();
	}
}

void TextureToolDialog::hideEvent(QHideEvent* event)
{
	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;
	auto* mainWindow = MainWindow::getInstance();

	for (auto& pair : data.pickedPolygons)
	{
		auto* polygon = pair.first;
		auto* brush = pair.second;
		brush->unselectPolygon(polygon);
	}

	data.pickedPolygons.clear();
	global->isSelectionForTextureToolActivated = false;
	global->isCameraForTextureToolActivated = false;
	global->m_editorMode = EditorMode::SELECTION_MODE;
	mainWindow->getSelectionToolButton()->setChecked(true);
	mainWindow->getTextureToolButton()->setChecked(false);
}

void TextureToolDialog::onPickedPolygonsChange(std::unordered_map<Types::Polygon*, Brush*>& pickedPolygons)
{
	if (pickedPolygons.size() < 1)
	{
		disableControls();
		return;
	}

	bool shiftX_sameVal = true;
	int shiftX_val = pickedPolygons.begin()->first->shift.x();
	bool shiftY_sameVal = true;
	int shiftY_val = pickedPolygons.begin()->first->shift.y();
	bool scaleX_sameVal = true;
	double scaleX_val = pickedPolygons.begin()->first->scale.x();
	bool scaleY_sameVal = true;
	double scaleY_val = pickedPolygons.begin()->first->scale.y();
	bool rotation_sameVal = true;
	int rotation_val = pickedPolygons.begin()->first->rotationAngle;
	bool textureId_sameVal = true;
	int textureId_val = pickedPolygons.begin()->first->textureId;

	for (auto it = ++pickedPolygons.begin(); it != pickedPolygons.end(); it++)
	{
		auto* polygon = it->first;

		if (shiftX_val != polygon->shift.x())
			shiftX_sameVal = false;
		if (shiftY_val != polygon->shift.y())
			shiftY_sameVal = false;
		if (!Helpers::areEqual(scaleX_val, polygon->scale.x()))
			scaleX_sameVal = false;
		if (!Helpers::areEqual(scaleY_val, polygon->scale.y()))
			scaleY_sameVal = false;
		if (rotation_val != polygon->rotationAngle)
			rotation_sameVal = false;
		if (textureId_val != polygon->textureId)
			textureId_sameVal = false;

		if (!shiftX_sameVal && !shiftY_sameVal && scaleX_sameVal && !scaleY_sameVal && !rotation_sameVal && !textureId_sameVal)
			break;
	}

	if (shiftX_sameVal)
	{
		m_shiftXControl->blockSignals(true);
			m_shiftXControl->setValue(shiftX_val);
		m_shiftXControl->blockSignals(false);

		m_shiftXControl->setDisabled(false);
	}
	else
	{
		m_shiftXControl->blockSignals(true);
		m_shiftXControl->setValue(shiftMin);
		m_shiftXControl->blockSignals(false);

		m_shiftX_isUndefined = true;
	}

	if (shiftY_sameVal)
	{
		m_shiftYControl->blockSignals(true);
			m_shiftYControl->setValue(shiftY_val);
		m_shiftYControl->blockSignals(false);

		m_shiftYControl->setDisabled(false);
	}
	else
	{
		m_shiftYControl->blockSignals(true);
			m_shiftYControl->setValue(shiftMin);
		m_shiftYControl->blockSignals(false);

		m_shiftY_isUndefined = true;
	}

	if (scaleX_sameVal)
	{
		m_scaleXControl->blockSignals(true);
			m_scaleXControl->setValue(scaleX_val);
		m_scaleXControl->blockSignals(false);

		m_scaleXControl->setDisabled(false);
	}
	else
	{
		m_scaleXControl->blockSignals(true);
		m_scaleXControl->setValue(scaleMin);
		m_scaleXControl->blockSignals(false);

		m_scaleX_isUndefined = true;
	}

	if (scaleY_sameVal)
	{
		m_scaleYControl->blockSignals(true);
			m_scaleYControl->setValue(scaleY_val);
		m_scaleYControl->blockSignals(false);

		m_scaleYControl->setDisabled(false);
	}
	else
	{
		m_scaleYControl->blockSignals(true);
			m_scaleYControl->setValue(scaleMin);
		m_scaleYControl->blockSignals(false);

		m_scaleY_isUndefined = true;
	}

	if (rotation_sameVal)
	{
		m_rotationControl->blockSignals(true);
			m_rotationControl->setValue(rotation_val);
		m_rotationControl->blockSignals(false);

		m_rotationControl->setDisabled(false);
	}
	else
	{
		m_rotationControl->blockSignals(true);
			m_rotationControl->setValue(rotationMin);
		m_rotationControl->blockSignals(false);

		m_rotation_isUndefined = true;
	}

	if (textureId_sameVal)
	{
		Texture* texture = ResourceManager::getTextureById(textureId_val);
		changePreviewTexture(*texture);
	}
	else
	{
		Texture dtTexture = ResourceManager::getTexture(QDir::currentPath() + "/assets/differing_textures.png", true);
		changePreviewTexture(dtTexture);
	}
}

void TextureToolDialog::handleUndefined(QSpinBox* control, int val, bool* isUndefined)
{
	if (*isUndefined)
	{
		val = 0;
		control->blockSignals(true);
		control->setValue(val);
		control->blockSignals(false);
		*isUndefined = false;
	}
}

void TextureToolDialog::handleUndefined(QDoubleSpinBox* control, double val, bool* isUndefined)
{
	if (*isUndefined)
	{
		val = 0;
		control->blockSignals(true);
		control->setValue(val);
		control->blockSignals(false);
		*isUndefined = false;
	}
}

void TextureToolDialog::handleShiftXChange(int val)
{
	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	handleUndefined(m_shiftXControl, val, &m_shiftX_isUndefined);

	if (!m_shiftX_editingStarted)
	{
		m_textureShiftXData = new Actions::TextureShiftData;

		for (auto& pair : data.pickedPolygons)
		{
			auto* polygon = pair.first;
			auto* brush = pair.second;
			m_textureShiftXData->insert(std::pair<Types::Polygon*, Actions::TextureShiftStruct>(polygon, { brush, polygon->shift }));
		}

		m_shiftX_editingStarted = true;
	}

	for (auto& [polygon, brush] : data.pickedPolygons)
	{
		polygon->shift = QVector2D(m_shiftXControl->value(), polygon->shift.y());
		brush->sendPolygonDataToGPU(polygon);
	}
}

void TextureToolDialog::handleShiftXEditingFinished()
{
	if (!m_shiftX_editingStarted)
		return;

	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	for (auto& pair : data.pickedPolygons)
	{
		auto* polygon = pair.first;
		QVector2D newShift(m_shiftXControl->value(), polygon->shift.y());
		(*m_textureShiftXData)[polygon].newShift = newShift;
	}

	ActionHistoryTool::addAction(Actions::textureshift_undo, Actions::textureshift_redo, Actions::textureshift_cleanup, m_textureShiftXData);

	m_textureShiftXData = nullptr;
	m_shiftX_editingStarted = false;
}

void TextureToolDialog::handleShiftYChange(int val)
{
	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	handleUndefined(m_shiftYControl, val, &m_shiftY_isUndefined);

	if (!m_shiftY_editingStarted)
	{
		m_textureShiftYData = new Actions::TextureShiftData;

		for (auto& pair : data.pickedPolygons)
		{
			auto* polygon = pair.first;
			auto* brush = pair.second;
			m_textureShiftYData->insert(std::pair<Types::Polygon*, Actions::TextureShiftStruct>(polygon, { brush, polygon->shift }));
		}

		m_shiftY_editingStarted = true;
	}

	for (auto& [polygon, brush] : data.pickedPolygons)
	{
		polygon->shift = QVector2D(polygon->shift.x(), m_shiftYControl->value());
		brush->sendPolygonDataToGPU(polygon);
	}
}

void TextureToolDialog::handleShiftYEditingFinished()
{
	if (!m_shiftY_editingStarted)
		return;

	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	for (auto& pair : data.pickedPolygons)
	{
		auto* polygon = pair.first;
		QVector2D newShift(polygon->shift.x(), m_shiftYControl->value());
		(*m_textureShiftYData)[polygon].newShift = newShift;
	}

	ActionHistoryTool::addAction(Actions::textureshift_undo, Actions::textureshift_redo, Actions::textureshift_cleanup, m_textureShiftYData);

	m_textureShiftYData = nullptr;
	m_shiftY_editingStarted = false;
}

void TextureToolDialog::handleScaleXChange(double val)
{
	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	handleUndefined(m_scaleXControl, val, &m_scaleX_isUndefined);

	if (!m_scaleX_editingStarted)
	{
		m_textureScaleXData = new Actions::TextureScaleData;

		for (auto& pair : data.pickedPolygons)
		{
			auto* polygon = pair.first;
			auto* brush = pair.second;
			m_textureScaleXData->insert(std::pair<Types::Polygon*, Actions::TextureScaleStruct>(polygon, { brush, polygon->scale }));
		}

		m_scaleX_editingStarted = true;
	}

	for (auto& [polygon, brush] : data.pickedPolygons)
	{
		polygon->scale = QVector2D(m_scaleXControl->value(), polygon->scale.y());
		brush->sendPolygonDataToGPU(polygon);
	}
}

void TextureToolDialog::handleScaleXEditingFinished()
{
	if (!m_scaleX_editingStarted)
		return;

	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	for (auto& pair : data.pickedPolygons)
	{
		auto* polygon = pair.first;
		QVector2D newScale(m_scaleXControl->value(), polygon->scale.y());
		(*m_textureScaleXData)[polygon].newScale = newScale;
	}

	ActionHistoryTool::addAction(Actions::texturescale_undo, Actions::texturescale_redo, Actions::texturescale_cleanup, m_textureScaleXData);

	m_textureScaleXData = nullptr;
	m_scaleX_editingStarted = false;
}

void TextureToolDialog::handleScaleYChange(double val)
{
	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	handleUndefined(m_scaleYControl, val, &m_scaleY_isUndefined);

	if (!m_scaleY_editingStarted)
	{
		m_textureScaleYData = new Actions::TextureScaleData;

		for (auto& pair : data.pickedPolygons)
		{
			auto* polygon = pair.first;
			auto* brush = pair.second;
			m_textureScaleYData->insert(std::pair<Types::Polygon*, Actions::TextureScaleStruct>(polygon, { brush, polygon->scale }));
		}

		m_scaleY_editingStarted = true;
	}

	for (auto& [polygon, brush] : data.pickedPolygons)
	{
		polygon->scale = QVector2D(polygon->scale.x(), m_scaleYControl->value());
		brush->sendPolygonDataToGPU(polygon);
	}
}

void TextureToolDialog::handleScaleYEditingFinished()
{
	if (!m_scaleY_editingStarted)
		return;

	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	for (auto& pair : data.pickedPolygons)
	{
		auto* polygon = pair.first;
		QVector2D newScale(polygon->scale.x(), m_scaleYControl->value());
		(*m_textureScaleYData)[polygon].newScale = newScale;
	}

	ActionHistoryTool::addAction(Actions::texturescale_undo, Actions::texturescale_redo, Actions::texturescale_cleanup, m_textureScaleYData);

	m_textureScaleYData = nullptr;
	m_scaleY_editingStarted = false;
}

void TextureToolDialog::handleRotationChange(int val)
{
	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	handleUndefined(m_rotationControl, val, &m_rotation_isUndefined);

	if (!m_rotation_editingStarted)
	{
		m_textureRotationData = new Actions::TextureRotationData;

		for (auto& pair : data.pickedPolygons)
		{
			auto* polygon = pair.first;
			auto* brush = pair.second;
			m_textureRotationData->insert(std::pair<Types::Polygon*, Actions::TextureRotationStruct>(polygon, { brush, polygon->rotationAngle }));
		}

		m_rotation_editingStarted = true;
	}

	for (auto& [polygon, brush] : data.pickedPolygons)
	{
		polygon->rotationAngle = m_rotationControl->value();
		brush->sendPolygonDataToGPU(polygon);
	}
}

void TextureToolDialog::handleRotationEditingFinished()
{
	if (!m_rotation_editingStarted)
		return;

	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	for (auto& pair : data.pickedPolygons)
	{
		auto* polygon = pair.first;
		(*m_textureRotationData)[polygon].newRotation = m_rotationControl->value();
	}

	ActionHistoryTool::addAction(Actions::texturerotation_undo, Actions::texturerotation_redo, Actions::texturerotation_cleanup, m_textureRotationData);

	m_textureRotationData = nullptr;
	m_rotation_editingStarted = false;
}

void TextureToolDialog::disableControls()
{
	m_shiftXControl->setDisabled(true);
	m_shiftYControl->setDisabled(true);
	m_scaleXControl->setDisabled(true);
	m_scaleYControl->setDisabled(true);
	m_rotationControl->setDisabled(true);

	m_shiftXControl->blockSignals(true);
		m_shiftXControl->setValue(shiftMin);
	m_shiftXControl->blockSignals(false);

	m_shiftYControl->blockSignals(true);
		m_shiftYControl->setValue(shiftMin);
	m_shiftYControl->blockSignals(false);

	m_scaleXControl->blockSignals(true);
		m_scaleXControl->setValue(scaleMin);
	m_scaleXControl->blockSignals(false);

	m_scaleYControl->blockSignals(true);
		m_scaleYControl->setValue(scaleMin);
	m_scaleYControl->blockSignals(false);

	m_rotationControl->blockSignals(true);
		m_rotationControl->setValue(rotationMin);
	m_rotationControl->blockSignals(false);
}

void TextureToolDialog::handleChangeTextureButtonClick()
{
	m_texturePickModal->open();
}

void TextureToolDialog::handleTexturePickModalSubmit(Texture texture)
{
	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;
	Actions::TexturePickingData* historyData = new Actions::TexturePickingData;

	GlobalData::applyingTexture = texture;
	changePreviewTexture(texture);

	if (this->shouldChangeDefaultTexture)
	{
		auto* brush = data.pickedPolygons.begin()->second;
		brush->setDefaultTexture(texture);
	}

	for (auto& pair : data.pickedPolygons)
	{
		auto* polygon = pair.first;
		auto* brush = pair.second;
		Texture oldTexture{ polygon->textureId, polygon->textureWidth, polygon->textureHeight };
		historyData->append({ brush, polygon, oldTexture, texture });
		brush->updatePolygonTexture(polygon, texture);
	}

	ActionHistoryTool::addAction(Actions::texturepicking_undo, Actions::texturepicking_redo, Actions::texturepicking_cleanup, historyData);
}

void TextureToolDialog::handleTexturePickModalCancel()
{
}
