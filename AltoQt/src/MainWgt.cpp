#include "MainWgt.hpp"
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QFileSystemModel>
#include <QListView>
#include <QTreeView>

//--------------------------------------------------------------------------------
MainWgt::MainWgt(QWidget*parent)
    : QWidget(parent)
    , _srcLed(this)
    , _brouseSrcBtn("...", this)
    , _dstLed(this)
    , _brouseDstBtn("...", this)
    , _encBtn("encrypt", this)
    , _decBtn("decrypt", this)
    //, _progressWgt(this)
{
    QHBoxLayout* btnLay = new QHBoxLayout();
    btnLay->addWidget(&_encBtn, 1);
    btnLay->addWidget(&_decBtn, 1);

    QGridLayout* lay = new QGridLayout(this);
    lay->addWidget(new QLabel("Source", this), 0, 0);
    lay->addWidget(&_srcLed         , 0, 1);
    lay->addWidget(&_brouseSrcBtn   , 0, 2);
    lay->addWidget(new QLabel("Destination", this), 1, 0);
    lay->addWidget(&_dstLed         , 1, 1);
    lay->addWidget(&_brouseDstBtn   , 1, 2);
    lay->addLayout(btnLay           , 2, 0, 1, 3);
    //lay->addWidget(&_progressWgt    , 3, 0, 1, 3);
    lay->setColumnStretch(1, 1);
    lay->setRowStretch(3, 1);

    _srcLed.setText("C:\\Prj\\Crypto\\.test\\input");
    _dstLed.setText("X:\\backup");

    connect(&_brouseSrcBtn, &QPushButton::released, this, &MainWgt::selectFile);
    connect(&_brouseDstBtn, &QPushButton::released, this, &MainWgt::selectFile);
    connect(&_encBtn, &QPushButton::released, this, [this](){emit sigX(_srcLed.text(), _dstLed.text(), 1);});
    connect(&_decBtn, &QPushButton::released, this, [this](){emit sigX(_srcLed.text(), _dstLed.text(), 0);});
}

class FileFilterProxyModel : public QSortFilterProxyModel
{
protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
    {
        QFileSystemModel* fileModel = qobject_cast<QFileSystemModel*>(sourceModel());
        return (fileModel!=NULL && fileModel->isDir(sourceModel()->index(sourceRow, 0, sourceParent))) || QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
    }
};

//--------------------------------------------------------------------------------
void MainWgt::selectFile(){
#if 0
    QFileDialog d;
    d.setFileMode(QFileDialog::DirectoryOnly);
    d.setOption(QFileDialog::DontUseNativeDialog, true);
#if 0
    FileFilterProxyModel* proxyModel = new FileFilterProxyModel;
    d.setProxyModel(nullptr);
    //d.setProxyModel(proxyModel);
#else
    QListView*l = d.findChild<QListView*>("listView");
    if(l){
        l->setSelectionMode(QAbstractItemView::MultiSelection);
     }
    QTreeView*t = d.findChild<QTreeView*>();
    if(t){
        t->setSelectionMode(QAbstractItemView::MultiSelection);
     }
#endif
    if(d.exec()){
        QString filename = d.selectedFiles()[0];
        if(sender() == &_brouseSrcBtn){
            _srcLed.setText(filename);
        }else{
            _dstLed.setText(filename);
        }
    }
    return;
#endif

    QString titleStr;
    if(sender() == &_brouseSrcBtn){
        titleStr = "Select source file or folder";
    }else{
        titleStr = "Select destination folder";
    }
    QString fileName = QFileDialog::getExistingDirectory(this, titleStr);
    if(!fileName.isEmpty()){
        if(sender() == &_brouseSrcBtn){
            _srcLed.setText(fileName);
        }else{
            _dstLed.setText(fileName);
        }
    }
}