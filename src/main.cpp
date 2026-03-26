#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QFileDialog>
#include <QDir>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Desktop Shortcut Creator");
    window.resize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&window);

    // Executable path
    QHBoxLayout *execLayout = new QHBoxLayout();
    QLabel *execLabel = new QLabel("Executable path:");
    execLayout->addWidget(execLabel);
    QLineEdit *execEdit = new QLineEdit();
    execLayout->addWidget(execEdit);
    QPushButton *execButton = new QPushButton("Browse...");
    execLayout->addWidget(execButton);
    layout->addLayout(execLayout);

    // Shortcut name
    QLabel *nameLabel = new QLabel("Shortcut name (without .desktop):");
    layout->addWidget(nameLabel);
    QLineEdit *nameEdit = new QLineEdit();
    layout->addWidget(nameEdit);

    // Icon path
    QHBoxLayout *iconLayout = new QHBoxLayout();
    QLabel *iconLabel = new QLabel("Icon path (optional):");
    iconLayout->addWidget(iconLabel);
    QLineEdit *iconEdit = new QLineEdit();
    iconLayout->addWidget(iconEdit);
    QPushButton *iconButton = new QPushButton("Browse...");
    iconLayout->addWidget(iconButton);
    layout->addLayout(iconLayout);

    QPushButton *createButton = new QPushButton("Create Shortcut");
    layout->addWidget(createButton);

    QObject::connect(createButton, &QPushButton::clicked, [&]() {
        QString execPath = execEdit->text().trimmed();
        QString name = nameEdit->text().trimmed();
        QString iconPath = iconEdit->text().trimmed();

        if (execPath.isEmpty() || name.isEmpty()) {
            QMessageBox::warning(&window, "Input Error", "Please enter both executable path and shortcut name.");
            return;
        }

        QString desktopPath = QDir::homePath() + "/Desktop/";
        QString desktopFile = desktopPath + name + ".desktop";

        QFile file(desktopFile);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(&window, "Error", "Could not create desktop file.");
            return;
        }

        QTextStream out(&file);
        out << "[Desktop Entry]\n";
        out << "Version=1.0\n";
        out << "Type=Application\n";
        out << "Name=" << name << "\n";
        out << "Exec=" << execPath << "\n";
        if (!iconPath.isEmpty()) {
            out << "Icon=" << iconPath << "\n";
        }
        out << "Terminal=false\n";
        out << "Categories=Utility;\n";

        file.close();

        // Make file executable
        QProcess process;
        process.start("chmod", QStringList() << "+x" << desktopFile);
        process.waitForFinished();

        QMessageBox::information(&window, "Success", "Desktop shortcut created at: " + desktopFile);
        
    });

    QObject::connect(execButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Select Executable", QDir::homePath(), "All Files (*)");
        if (!fileName.isEmpty()) {
            execEdit->setText(fileName);
        }
    });

    QObject::connect(iconButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(&window, "Select Icon", QDir::homePath(), "Images (*.png *.svg *.xpm *.ico);;All Files (*)");
        if (!fileName.isEmpty()) {
            iconEdit->setText(fileName);
        }
    });

    window.show();
    return app.exec();
}