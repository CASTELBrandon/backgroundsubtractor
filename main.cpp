#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <vector>

#include "mainwindow.h"
#include "BackgroundSubtractor.h"

int main(int argc, char *argv[])
{
    // Init application
    QApplication a(argc, argv);
    QApplication::setApplicationName("BackgroundSubtractor");
    QApplication::setApplicationVersion("1.0");
    MainWindow w;

    // Check if we have command line arguments
    auto args = a.arguments();
    args.removeAt(0); // The first argument is the executable path
    if(!args.empty()){
        QCommandLineParser parser;
        parser.setApplicationDescription("This program allows to subtract the background of a sequence of images.");
        parser.addHelpOption();
        parser.addVersionOption();

        QCommandLineOption inputOption(QStringList() << "i" << "input", "Input folder which contains multiple sequences of camera images.", "path");
        parser.addOption(inputOption);

        QCommandLineOption outputOption(QStringList() << "o" << "output", "Output folder which will contain all the subtracted images.", "path");
        parser.addOption(outputOption);

        parser.process(a);

        // Check if input and output folder are set
        if(!(parser.isSet(inputOption) && parser.isSet(outputOption))){
            qCritical() << "Error : if you are using the command line system, the input path and the output path must be specified.";
            return -1;
        }

        QString input = parser.value(inputOption);
        qDebug() << "Input folder : " << input;

        QString output = parser.value(outputOption);
        qDebug() << "Output folder : " << output;
    }
    // If no command lines we show the window
    else{
        w.show();
    }

    QDir dirSubject("E:/Dossiers personnels/Cours et projet/Entretiens/C++/Kinovis/Ressources/Capture-sequence/cam-007");
    QDir dirBackground("E:/Dossiers personnels/Cours et projet/Entretiens/C++/Kinovis/Ressources/Background-sequence/cam-007");
    QFileInfoList subjectSequence = dirSubject.entryInfoList(QStringList() << "*.png" << "*.PNG", QDir::Files);
    QFileInfoList backgroundSequence = dirBackground.entryInfoList(QStringList() << "*.png" << "*.PNG", QDir::Files);

    return a.exec();
}
