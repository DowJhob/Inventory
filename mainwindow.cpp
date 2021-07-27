#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSizePolicy"
#include "QHeaderView"
#include "QToolButton"
#include "QWidgetAction"
//#include "QToolButton"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect(this, &QWidget::)

    create_DB_worker("PlayerName");

    ui->gridLayout->addWidget(&inventory_table, 1, 0);
    ui->gridLayout->addWidget(&source_table, 1, 1);
    ui->gridLayout->addWidget(&menu_button, 1, 2);




    menu_button.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    menu_button.setFixedSize(100, 100);
    menu_button.setText("Главное меню");
    connect(&menu_button, &QPushButton::clicked, this, [this](){
        enableInterface(false);
    _main_menu->exec();
    });

    inventory_table.setFixedSize(600, 600);
    source_table.setFixedSize(200, 200);

    inventory_table.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    source_table.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    inventory_table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    inventory_table.horizontalHeader()->resizeSections(QHeaderView::Stretch);
    inventory_table.verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    inventory_table.verticalHeader()->resizeSections(QHeaderView::Stretch);
    inventory_table.horizontalHeader()->setVisible(false);
    inventory_table.verticalHeader()->setVisible(false);
    source_table.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    source_table.horizontalHeader()->resizeSections(QHeaderView::Stretch);
    source_table.verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    source_table.verticalHeader()->resizeSections(QHeaderView::Stretch);
    source_table.horizontalHeader()->setVisible(false);
    source_table.verticalHeader()->setVisible(false);

    inventory_table.setColumnCount(3);
    inventory_table.setRowCount(3);
    source_table.setColumnCount(1);
    source_table.setRowCount(1);

    inventory_table.setSelectionMode(QAbstractItemView::SingleSelection);
    inventory_table.setShowGrid(true);
    inventory_table.setIconSize(QSize( 200,200));
    source_table.setIconSize(QSize( 200,200));


    inventory_table.setDragEnabled(true);
    inventory_table.setAcceptDrops(true);
    inventory_table.setDragDropMode(QAbstractItemView::DragDrop);

    connect( &inventory_table, &inventoryWidget::itemDestroy_sig, this, &MainWindow::itemDestroy );
    source_table.setDragDropMode( QAbstractItemView::DragOnly);
    source_table.setDragEnabled(true);
    inventoryItem *item = new inventoryItem(itemType::apple);
    source_table.setItem(0,0, item);

    show();
    _main_menu = new MainMenu(this);
    connect( _main_menu, &MainMenu::NewGame_sig, this, &MainWindow::NewGame );
    connect( _main_menu, &MainMenu::ExitGame_sig, this, &MainWindow::quit );
    connect( _main_menu, &MainMenu::LoadGame_sig, this, &MainWindow::req_get_inventory_sig );
    connect( _main_menu, &MainMenu::SaveGame_sig, this, &MainWindow::save_inventory );

player.setMedia(QUrl("qrc:/apple.mp3"));

    //player = new QMediaPlayer;

    //playlist = new QMediaPlaylist(player);
    //playlist->addMedia(QUrl("http://example.com/myfile1.mp3"));

}

MainWindow::~MainWindow()
{
    _main_menu->deleteLater();
    safe_delete_db_worker();
    delete ui;
}



