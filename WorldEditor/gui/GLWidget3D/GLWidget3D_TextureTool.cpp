#include "GLWidget3D.h"
#include "../../common/GlobalData.h"
#include "../MainWindow.h"

void GLWidget3D::processTextureTool()
{
	auto global = GlobalData::getInstance();
	auto& data = global->textureToolData;
	auto* mainWindow = MainWindow::getInstance();
	auto* ttDialog = mainWindow->getTextureToolDialog();

	if (isWidgetActive())
	{
		if (m_inputData.leftMouseDown == ButtonDownState::DOWN_NOT_PROCESSED)
		{
			Brush* brush;
			Types::Polygon* polygon;
			bool isBrushPicked = pickBrush(&brush, &polygon, nullptr, nullptr);

			if (!isBrushPicked || !global->isSelectionForTextureToolActivated)
				return;

			ttDialog->shouldChangeDefaultTexture = false;

			if (m_inputData.keyCtrl == ButtonDownState::DOWN_NOT_PROCESSED || m_inputData.keyCtrl == ButtonDownState::DOWN_PROCESSED)
			{
				if (data.pickedPolygons.find(polygon) != data.pickedPolygons.end())
				{
					data.pickedPolygons.erase(polygon);
					brush->unselectPolygon(polygon);
					ttDialog->onPickedPolygonsChange(data.pickedPolygons);
				}
				else
				{
					data.pickedPolygons[polygon] = brush;
					brush->selectPolygon(polygon);
					ttDialog->onPickedPolygonsChange(data.pickedPolygons);
				}
			}
			else
			{
				for (auto& pair : data.pickedPolygons)
				{
					brush->unselectPolygon(pair.first);
				}

				data.pickedPolygons.clear();

				brush->selectPolygon(polygon);
				data.pickedPolygons[polygon] = brush;
				ttDialog->onPickedPolygonsChange(data.pickedPolygons);
			}
		}
	}
}
