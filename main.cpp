#include <QFile>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <poppler-qt5.h>
#include <iostream>

#include "extractannotation.h"

int main(int argc, char* argv[]) {

	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("extract-annotation");
	QCoreApplication::setApplicationVersion("1.0");


    QString synopsis = "Extracts text annotations from pdf files, formatted in markdown";
    QString usage    = "If no pdf files are given, reads standard input for pdf file";

	QCommandLineParser parser;
    parser.setApplicationDescription("Synopsis: " + synopsis + "\n" + "Behavior: " + usage);
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption verboseOption(QStringList() << "r" << "raw", "Output annotations as raw text");
	parser.addOption(verboseOption);

    parser.addPositionalArgument("filename", "PDF files to process");

    parser.process(app);

    bool        verbose = !parser.isSet(verboseOption);
    QStringList args    = parser.positionalArguments();

//    auto f = [](int a, int b) -> int { return a + b; };
    auto docNameVerbose = [&verbose](QString& str) -> std::string {
        return (verbose ? "##  " + str + "\n" : "").toStdString();
    };

    if (args.isEmpty()) {

        QFile in;
        in.open(stdin, QIODevice::ReadOnly);
        Poppler::Document* doc = Poppler::Document::loadFromData(in.readAll());
        std::cout << extractFromDoc(doc, verbose);
    } else {

        for (int i = 0; i < args.length(); ++i) {

            Poppler::Document* doc = Poppler::Document::load(args[i]);
            std::cout << docNameVerbose(args[i]) << extractFromDoc(doc, verbose);
        }
    }

    return 0;
}
