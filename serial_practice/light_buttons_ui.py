# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'light_buttons.ui'
#
# Created: Mon Aug  3 21:26:29 2015
#      by: PyQt4 UI code generator 4.11.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui
import sys
import visa

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

rm = visa.ResourceManager()
ard = rm.open_resource(u'ASRL3::INSTR')

class Ui_Form(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)
        self.setupUi(self)

    def setupUi(self, Form):
        Form.setObjectName(_fromUtf8("Form"))
        Form.resize(471, 354)
        self.horizontalLayout = QtGui.QHBoxLayout(Form)
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.verticalLayout_3 = QtGui.QVBoxLayout()
        self.verticalLayout_3.setObjectName(_fromUtf8("verticalLayout_3"))
        self.turn_on = QtGui.QPushButton(Form)
        self.turn_on.setObjectName(_fromUtf8("turn_on"))
        self.verticalLayout_3.addWidget(self.turn_on)
        self.turn_off = QtGui.QPushButton(Form)
        self.turn_off.setObjectName(_fromUtf8("turn_off"))
        self.verticalLayout_3.addWidget(self.turn_off)
        self.horizontalLayout.addLayout(self.verticalLayout_3)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem1)

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "Lights Camera Action", None))
        self.turn_on.setText(_translate("Form", "Turn On", None))
        self.turn_off.setText(_translate("Form", "Turn Off", None))
        self.turn_on.clicked.connect(self.light_on)
        self.turn_off.clicked.connect(self.light_off)

    def light_on(self):
        ard.write('i')

    def light_off(self):
        ard.write('o')

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    ex = Ui_Form()
    ex.show()
    sys.exit(app.exec_())
