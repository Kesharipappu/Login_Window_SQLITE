#include "utils.h"

void applyMessageBoxStyle(QMessageBox &msgBox) {
    msgBox.setStyleSheet("QMessageBox { background-color:#729FCF; }"  //  Periwinkle Blue background
                         "QLabel { color: #1a1a2a; }"  // Dark blue text
                         "QPushButton { color: #1a237e; background-color: #ffffff; }"  // White buttons with dark blue text
                         "QPushButton:hover { background-color: #e0e0e0; }");  // Light gray hover effect
}
