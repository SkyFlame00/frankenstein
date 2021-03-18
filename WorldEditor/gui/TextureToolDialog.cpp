#include "TextureToolDialog.h"
#include "../common/GlobalData.h"
#include <QDebug>
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
	m_shiftXControl->setMinimum(intMin);
	m_shiftXControl->setMaximum(intMax);
	m_shiftXLayout->addWidget(m_shiftXLabel);
	m_shiftXLayout->addWidget(m_shiftXControl);
	m_shiftXContainer->setLayout(m_shiftXLayout);

	/* Shift Y */
	QWidget* m_shiftYContainer = new QWidget;
	QHBoxLayout* m_shiftYLayout = new QHBoxLayout;
	QLabel* m_shiftYLabel = new QLabel("Shift Y");
	m_shiftYControl = new QSpinBox;
	m_shiftYControl->setSpecialValueText("-");
	m_shiftYControl->setMinimum(intMin);
	m_shiftYControl->setMaximum(intMax);
	m_shiftYLayout->addWidget(m_shiftYLabel);
	m_shiftYLayout->addWidget(m_shiftYControl);
	m_shiftYContainer->setLayout(m_shiftYLayout);

	/* Scale X */
	QWidget* m_scaleXContainer = new QWidget;
	QHBoxLayout* m_scaleXLayout = new QHBoxLayout;
	QLabel* m_scaleXLabel = new QLabel("Scale X");
	m_scaleXControl = new QSpinBox;
	m_scaleXControl->setMinimum(intMin);
	m_scaleXControl->setMaximum(intMax);
	m_scaleXControl->setSpecialValueText("-");
	m_scaleXLayout->addWidget(m_scaleXLabel);
	m_scaleXLayout->addWidget(m_scaleXControl);
	m_scaleXContainer->setLayout(m_scaleXLayout);

	/* Scale Y */
	QWidget* m_scaleYContainer = new QWidget;
	QHBoxLayout* m_scaleYLayout = new QHBoxLayout;
	QLabel* m_scaleYLabel = new QLabel("Scale Y");
	m_scaleYControl = new QSpinBox;
	m_scaleYControl->setMinimum(intMin);
	m_scaleYControl->setMaximum(intMax);
	m_scaleYControl->setSpecialValueText("-");
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

	for (auto it = ++pickedPolygons.begin(); it != pickedPolygons.end(); it++)
	{
		auto* polygon = it->first;

		if (shiftX_val != polygon->shift.x())
			shiftX_sameVal = false;

		if (!shiftX_sameVal)
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
			m_shiftXControl->setValue(intMin);
		m_shiftXControl->blockSignals(false);

		m_shiftX_isUndefined = true;
	}
}

void TextureToolDialog::handleShiftXChange(int val)
{
	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;

	if (m_shiftX_isUndefined)
	{
		val = 0;
		m_shiftXControl->blockSignals(true);
		m_shiftXControl->setValue(val);
		m_shiftXControl->blockSignals(false);
		m_shiftX_isUndefined = false;
	}

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

	for (auto pair : data.pickedPolygons)
	{
		auto* polygon = pair.first;
		auto* brush = pair.second;
		auto& vbo = brush->getTrianglesVbo();
		int size = polygon->triangles.size() * 3 * 14;
		float* polygonData = new float[size];
		int i = 0;

		polygon->shift = QVector2D(m_shiftXControl->value(), polygon->shift.y());
		brush->makePolygonVertices(polygon, i, polygonData);
		vbo.subdata(polygon->begin, size * sizeof(float), polygonData);
		delete[] polygonData;
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

void TextureToolDialog::disableControls()
{
	m_shiftXControl->setDisabled(true);
	m_shiftYControl->setDisabled(true);
	m_scaleXControl->setDisabled(true);
	m_scaleYControl->setDisabled(true);

	m_shiftXControl->blockSignals(true);
		m_shiftXControl->setValue(intMin);
	m_shiftXControl->blockSignals(false);

	m_shiftXControl->blockSignals(true);
		m_shiftXControl->setValue(intMin);
	m_shiftXControl->blockSignals(false);

	m_scaleXControl->blockSignals(true);
		m_scaleXControl->setValue(intMin);
	m_scaleXControl->blockSignals(false);

	m_scaleYControl->blockSignals(true);
		m_scaleYControl->setValue(intMin);
	m_scaleYControl->blockSignals(false);
}
