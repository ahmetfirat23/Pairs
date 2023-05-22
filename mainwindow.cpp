#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>

int score = 0;
int noTriesLeft = 50;
std::string lastCard = ""; // last opened card's name
int openCards = 0;
// original list for names
std::vector<std::string> names= {"Dog","Cat","Bird","Elephant","Tiger","Lion","Bear","Gorilla","Monkey","Dolphin","Whale","Fish","Snake","Deer" ,"Giraffe"
                                  ,"Dog","Cat","Bird","Elephant","Tiger","Lion","Bear","Gorilla","Monkey","Dolphin","Whale","Fish","Snake","Deer","Giraffe"};
std::vector<std::string> game_names; // names of dealed cards
QPushButton* cards[30];
QPushButton* openCardsArr[2];


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    srand(time(NULL));
    ui->setupUi(this);
    ui->scoreLabel->setText(QString::number(score));
    ui->tryLabel->setText(QString::number(noTriesLeft));

    std::vector<std::string> game_set(names); // copy the original names for dealing

    for(int i = 0; i < 30; i++){
        QString buttonName = "pushButton_" + QString::number(i);
        cards[i] = MainWindow::findChild<QPushButton *>(buttonName);
        cards[i]->setText("?");

        // deal the cards
        int idx = rand() % game_set.size();
        std::string cardValue = game_set[idx];
        game_set.erase(game_set.begin() + (idx));
        game_names.push_back(cardValue);
        connect(cards[i], SIGNAL(released()), this,
                SLOT(CardPressed()));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CardPressed(){
    // get the name of the card
    QPushButton* button = (QPushButton *) sender();
    QString buttonStr = button->objectName();
    int idx = buttonStr.split("_")[1].toInt();
    std::string buttonVal = game_names[idx];

    button->setText(QString::fromStdString(buttonVal));
    button->setEnabled(false);

    if (openCards == 0){ // logic when this is the first card
        if (openCardsArr[0]!=nullptr){ // if user didn't succeed previous in previous try
            openCardsArr[0]->setEnabled(true);
            openCardsArr[1]->setEnabled(true);
            openCardsArr[0]->setText("?");
            openCardsArr[1]->setText("?");
        }
        openCardsArr[0] = button;
        openCards++;
    }
    else if(openCards == 1){ // logic when this is the second card
        noTriesLeft--;
        ui->tryLabel->setText(QString::number(noTriesLeft));
        openCardsArr[1] = button;

        // get prev card's name
        QString prevButtonStr = openCardsArr[0]->objectName();
        int idx = prevButtonStr.split("_")[1].toInt();
        std::string prevButtonVal = game_names[idx];

        if(buttonVal == prevButtonVal){ // if user succeed
            score++;
            ui->scoreLabel->setText(QString::number(score));
            openCardsArr[0] = nullptr; // logic for detecting success
        }
        openCards--;
    }

    if(score==15){
        ui->newButton->setText("You Win! Play Again."); // change the new game button for win message
    }
    else if(noTriesLeft==0){
        // if you played minesweeper you should know this is the way
        ui->newButton->setText("Game over! Play Again."); // change the new game button for loss message
        for(int i = 0; i < 30; i++){ // open all cards to shame the player
            std::string buttonVal = game_names[i];
            cards[i]->setText(QString::fromStdString(buttonVal));
            cards[i]->setEnabled(false);
        }
    }

}


void MainWindow::on_newButton_released(){
    ui->newButton->setText("New Game"); // clear previous message
    // reset the game
    openCardsArr[0] = nullptr;
    openCards = 0;
    score = 0;
    noTriesLeft = 50;
    ui->scoreLabel->setText(QString::number(score));
    ui->tryLabel->setText(QString::number(noTriesLeft));
    game_names.clear();
    std::vector<std::string> game_set(names);
    for(int i = 0; i < 30; i++){
        cards[i]->setText("?");
        cards[i]->setEnabled(true);
        int idx = rand() % game_set.size();
        std::string cardValue = game_set[idx];
        game_set.erase(game_set.begin() + (idx));
        game_names.push_back(cardValue);
    }
}
