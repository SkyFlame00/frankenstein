#include "TextureToolDialog.h"
#include "../common/GlobalData.h"
#include "../common/helpers.h"
#include <limits>
#include "../common/ActionHistoryTool.h"
#include "MainWindow.h"

TextureToolDialog::TextureToolDialog(QWidget* parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	setWindowTitle("Texture tool");
	setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);

	/* Main layouts */
	QHBoxLayout* m_mainLayout = new QHBoxLayout;
	QHBoxLayout* m_texturePreviewLayout = new QHBoxLayout;
	QGridLayout* m_textureParamsLayout = new QGridLayout;
	m_textureParamsLayout->setSizeConstraint(QLayout::SetFixedSize);

	/* Texture preview */
	QWidget* m_texturePreviewContainer = new QWidget;
	m_texturePreviewContainer->setLayout(m_texturePreviewLayout);
	m_mainLayout->addWidget(m_texturePreviewContainer);
	
	/* Change texture button */
	m_changeTextureButton = new QPushButton;
	m_changeTextureButton->setText("Change texture");
	m_texturePreviewLayout->addWidget(m_changeTextureButton);

	/* Texture parameters */
	QWidget* m_textureParamsContainer = new QWidget;
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

	/* Populate with created widgets */
	m_textureParamsLayout->addWidget(m_shiftXContainer, 0, 0, 1, 1);
	m_textureParamsLayout->addWidget(m_shiftYContainer, 0, 1, 1, 1);
	m_textureParamsLayout->addWidget(m_scaleXContainer, 1, 0, 1, 1);
	m_textureParamsLayout->addWidget(m_scaleYContainer, 1, 1, 1, 1);

	setLayout(m_mainLayout);
	connect(m_shiftXControl, qOverload<int>(&QSpinBox::valueChanged), this, &TextureToolDialog::handleShiftXChange);
	connect(m_shiftXControl, &QSpinBox::editingFinished, this, &TextureToolDialog::handleShiftXEditingFinished);
	connect(m_shiftYControl, qOverload<int>(&QSpinBox::valueChanged), this, &TextureToolDialog::handleShiftYChange);
	connect(m_shiftYControl, &QSpinBox::editingFinished, this, &TextureToolDialog::handleShiftYEditingFinished);
	connect(m_scaleXControl, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &TextureToolDialog::handleScaleXChange);
	connect(m_scaleXControl, &QDoubleSpinBox::editingFinished, this, &TextureToolDialog::handleScaleXEditingFinished);
	connect(m_scaleYControl, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &TextureToolDialog::handleScaleYChange);
	connect(m_scaleYControl, &QDoubleSpinBox::editingFinished, this, &TextureToolDialog::handleScaleYEditingFinished);
}

void TextureToolDialog::showEvent(QShowEvent* event)
{
	auto* global = GlobalData::getInstance();
	auto& sdata = global->m_selectionToolData;
	auto& tdata = global->textureToolData;

	if (sdata.renderable)
	{
		auto* brush = sdata.renderable;
		sdata.renderable->m_selected = false;
		sdata.renderable = nullptr;

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

		if (!shiftX_sameVal && !shiftY_sameVal && scaleX_sameVal && !scaleY_sameVal)
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

void TextureToolDialog::disableControls()
{
	m_shiftXControl->setDisabled(true);
	m_shiftYControl->setDisabled(true);
	m_scaleXControl->setDisabled(true);
	m_scaleYControl->setDisabled(true);

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
}
