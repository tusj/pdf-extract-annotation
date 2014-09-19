#ifndef EXTRACTANNOTATION_H
#define EXTRACTANNOTATION_H

#include <poppler-qt5.h>
#include <sstream>

std::string extractAnnotation(Poppler::Annotation* annot, Poppler::Page* page);
std::string extractFromDoc(Poppler::Document* doc, bool verbose);

#endif // EXTRACTANNOTATION_H
