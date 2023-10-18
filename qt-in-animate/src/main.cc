#include <animate/jsapi.h>
#include <QtWidgets>
#include <QtGui/6.5.3/QtGui/private/qcssparser_p.h>
#include <Windows.h>
#include <qregularexpression.h>
#include <string>
#include <map>

QString PaletteReplace(QString input) {
    QString result = input;

    QCss::Parser parser(input);
    while(parser.testRuleset()) {
        QCss::StyleRule style_rule;
        if(!parser.parseRuleset(&style_rule)) continue;

        for(auto& selector : style_rule.selectors) {
            if(selector.basicSelectors.size() != 1 || selector.basicSelectors[0].elementName != "AnimatePalette") continue;
            for(auto& declaration : style_rule.declarations) {
                QString values = "";
                for(auto& value : declaration.d->values) {
                    values += value.toString();
                }
                QString regex = "animate\\(" + declaration.d->property + "\\)";
                result.replace(QRegularExpression(regex), values);
            }
        }
    }
    return result;
}

QString LoadTheme(QString path) { 
    QFile base_file(":/themes/base.qss");
    base_file.open(QFile::ReadOnly);
    
    QFile theme_file(path);
    theme_file.open(QFile::ReadOnly);

    QStringList stylesheets = { base_file.readAll(), theme_file.readAll() };

    QString processed = PaletteReplace(stylesheets.join("\n"));
    processed.replace(QRegularExpression("\n"), "");
    return processed;
}

ANIMATE_INIT(Init)
void Init() {
    QString stylesheet = LoadTheme(":/themes/dark.qss");
    MessageBoxW(nullptr, stylesheet.toStdWString().c_str(), L"compiled", MB_OK);

    int argc = 1;
    char* argv[] = { (char*) "qt-in-animate" };
    QApplication app(argc, argv);
    app.setStyleSheet(stylesheet);

    QMessageBox message_box(QMessageBox::Information, "qt-in-animate", "Test message!", QMessageBox::Ok | QMessageBox::Cancel);
    message_box.exec();
}

ANIMATE_TERMINATE(Terminate)
int Terminate() {
    return 0;
}
