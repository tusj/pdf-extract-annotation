#include "extractannotation.h"

std::string replace(const std::string& str, const char* searchFor, const char* replaceWith);

std::string extractFromDoc(Poppler::Document* doc, bool verbose) {
    std::ostringstream str;

    for (int i = 0; i < doc->numPages(); i++) {
        Poppler::Page* page = doc->page(i);
        QList<Poppler::Annotation*> annots = page->annotations();

        std::ostringstream pageData;
        bool empty = true;
        for(int i = 0; i < annots.length(); ++i) {
            std::string data = extractAnnotation(annots[i], page);
            if (!data.empty()) {

                if (verbose) {
                    if (data[data.length()-1] == '\n') {
                        data = data.substr(0, data.length()-2);
                    }
                    data = replace(data, "\n", "\n    ");
                }
                pageData << (verbose ? "-   " : "") << data << (verbose ? "\n" : "\n\n");
                empty = false;
            }
        }

        if (!empty && verbose) {
            str << "### page " << i + 1 << std::endl;
        }
        str << pageData.str() << std::endl;
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

            QPointF start = points[0];
            QPointF end = points[2];
            QSize pageSize = page->pageSize();
            start.setX(start.x() * pageSize.width ());
            start.setY(start.y() * pageSize.height());
            end  .setX(end  .x() * pageSize.width ());
            end  .setY(end  .y() * pageSize.height());
            QRectF rect(start, end);


            std::string text = page->text(rect).toStdString();
            if (text != "") {
                str << text;
            }
            if (i < quads.length() - 1) {
                str << std::endl;
            }
        }
    }
    if (annotText != 0) {
        std::string contents = annotText->contents().toStdString();
        if (contents != "") {
            str << contents;
        }
    }

    return str.str();
}

std::string replace(const std::string& str, const char* searchFor, const char* replaceWith) {
    size_t index = 0;
    std::string data = str;
    while (true) {
        index = data.find(searchFor, index);
        if (index == std::string::npos) break;

        data.replace(index, 2, replaceWith);
        index += 2;
    }

    return data;
}
