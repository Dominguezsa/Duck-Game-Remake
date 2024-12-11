#include "WrapperLobbyApp.h"

bool WrapperLobbyApp::notify(QObject *receiver, QEvent *event) {
    try {
        return QApplication::notify(receiver, event); // Llama al manejador de eventos estándar
    }
    catch (const std::exception& e) {
        // Catch any exception thrown and handle the error
        qDebug() << "Exception caught: " << e.what();
        // Show a message box with the error message
        QMessageBox::warning(nullptr, "Error", "An unexpected error occurred: " + QString::fromStdString(e.what()));
        return false; // Return false to indicate that the exception has been handled
    }
}