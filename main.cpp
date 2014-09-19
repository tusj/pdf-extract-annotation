#include <poppler-qt5.h>
#include <iostream>
#include <ostream>
#include <iterator>
#include <QFile>

std::string extractAnnotation(Poppler::Annotation* annot, Poppler::Page* page);
std::string extractFromDoc(Poppler::Document* doc);


int main(int argc, char* argv[]) {

    if (argc == 1) {
        QFile in;
        in.open(stdin, QIODevice::ReadOnly);
        Poppler::Document* doc = Poppler::Document::loadFromData(in.readAll());
        std::cout << extractFromDoc(doc);
    } else {
        for (int i = 1; i < argc; ++i) {
            Poppler::Document* doc = Poppler::Document::load(argv[i]);
            std::cout << extractFromDoc(doc);
        }
    }
}

std::string extractFromDoc(Poppler::Document* doc) {
    std::ostringstream str;

    for (int i = 0; i < doc->numPages(); i++) {

        Poppler::Page* page = doc->page(i);
        QList<Poppler::Annotation*> annots = page->annotations();

        for(int j = 0; j < annots.length(); ++j) {

            str << extractAnnotation(annots[j], page);
        }
    }
    return str.str();
}

std::string extractAnnotation(Poppler::Annotation* annot, Poppler::Page* page) {

    Poppler::HighlightAnnotation*      annotHL     = dynamic_cast<Poppler::HighlightAnnotation*>      (annot);
    Poppler::TextAnnotation*           annotText   = dynamic_cast<Poppler::TextAnnotation*>           (annot);

    std::ostringstream str;

    if (annotHL != 0) {
        QList<Poppler::HighlightAnnotation::Quad> quads = annotHL->highlightQuads();
        for (int i = 0; i < quads.length(); i++) {

            QPointF* points = quads[i].points;
            QSize size = page->pageSize();

            QPointF start = points[0];
            QPointF end = points[2];

            start.setX(start.x() * size.width ());
            start.setY(start.y() * size.height());
            end  .setX(end  .x() * size.width ());
            end  .setY(end  .y() * size.height());
            QRectF rect(start, end);


            std::string text = page->text(rect).toStdString();
            str << text << std::endl;
        }
    }
    if (annotText != 0) {
        std::string contents = annotText->contents().toStdString();
        if (contents != "") {
            str << contents << std::endl;
        }
    }

    return str.str();
}
