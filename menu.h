#ifndef MENU_H
#define MENU_H

#include <QDialog>
#include <QObject>
#include <QGridLayout>
#include <QPushButton>

class MainMenu : public QDialog
{
    Q_OBJECT
public:
    MainMenu(QWidget *w = nullptr)
    {
        setParent(w);
        setWindowFlags(Qt::SplashScreen);

        setFixedSize(w->size().width()-200, 200);
        QGridLayout* layout = new QGridLayout;
        QPushButton* NewBtn = new QPushButton( "New", this);
        NewBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //connect( NewBtn, &QPushButton::clicked, this, &QDialog::accept );
        connect( NewBtn, &QPushButton::clicked, this, &MainMenu::NewGame_sig );
        layout->addWidget( NewBtn, 0, 0 );        
        QPushButton* LoadBtn = new QPushButton( "Load", this );
        LoadBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect( LoadBtn, &QPushButton::clicked, this, &MainMenu::LoadGame_sig );
        layout->addWidget( LoadBtn, 1, 0);
        QPushButton* SaveBtn = new QPushButton( "Save", this );
        SaveBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect( SaveBtn, &QPushButton::clicked, this, &MainMenu::SaveGame_sig );
        layout->addWidget( SaveBtn, 2, 0);
        QPushButton* ExitBtn = new QPushButton( "Exit", this );
        ExitBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect( ExitBtn, &QPushButton::clicked, this, &MainMenu::ExitGame_sig );
        layout->addWidget( ExitBtn, 3, 0);
        setLayout( layout );

    }
signals:
    void NewGame_sig();
    void ExitGame_sig();
    void LoadGame_sig();
    void SaveGame_sig();
};

#endif // MENU_H
