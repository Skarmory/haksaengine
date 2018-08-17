#pragma once

#include <vector>

#include <QWidget>
#include <QPushButton>
#include "ui_unitpalettewidget.h"

#include "event/event_handler.h"
#include "io/blueprint.h"

class UnitPaletteWidget : public QWidget, public EventHandler
{
	Q_OBJECT

public:
	UnitPaletteWidget(QWidget *parent = Q_NULLPTR);
	~UnitPaletteWidget();

	void init(void);
	void on_event(Event e) override;

	void set_units(const std::vector<const Blueprint*>& units);
	void set_objects(const std::vector<const Blueprint*>& objects);

	void update_all(void);

private slots:
	void swap_to_unit_place(void);
	void swap_to_object_place(void);

private:
	Ui::UnitPaletteWidget ui;

	QButtonGroup units_button_group;
	QButtonGroup object_button_group;

	const std::string unit_path = "blueprints/units/";
	const std::string unit_dir = "units/";
	const std::string object_dir = "objects/";
	const std::string icon_path = "textures/icons/";

	std::vector<const Blueprint*> unit_blueprints;
	std::vector<const Blueprint*> object_blueprints;
};
