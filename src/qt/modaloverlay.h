// Copyright (c) 2016-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_MODALOVERLAY_H
#define BITCOIN_QT_MODALOVERLAY_H

#include <qt/downloader.h>

#include <util.h>
#include <qt/guiutil.h>
#include <QDateTime>
#include <QWidget>
#include <QProcess>
#include <QThread>

#include <thread>

class PlatformStyle;

//! The required delta of headers to the estimated number of available headers until we show the IBD progress
static constexpr int HEADER_HEIGHT_DELTA_SYNC = 24;

namespace Ui {
    class ModalOverlay;
}

/** Modal overlay to display information about the chain-sync state */
class ModalOverlay : public QWidget
{
    Q_OBJECT

    enum QuickSyncStatus
    {
        PREPARING=0,
        DOWNLOADING=1,
        TIMEOUT=2,
        DECODING=3,
        EXTRACTING=4,
        CANCELED=-1
    };

public:
    explicit ModalOverlay(const PlatformStyle *_platformStyle, QWidget *parent);
    ~ModalOverlay();

public Q_SLOTS:
    void tipUpdate(int count, const QDateTime& blockDate, double nVerificationProgress);
    void setKnownBestHeight(int count, const QDateTime& blockDate);

    void toggleVisibility();
    // will show or hide the modal layer
    void showHide(bool hide = false, bool userRequested = false);
    void closeClicked();
    bool isLayerVisible() const { return layerIsVisible; }
    /** DeepOnion Theme Change */
    void refreshStyle();

protected:
    bool eventFilter(QObject * obj, QEvent * ev);
    bool event(QEvent* ev);

private Q_SLOTS:
    void onQuickSyncClicked();
    void onCancelButtonClicked();
    void onUpdateProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();
    void onProgessBarUpdated(qint64, qint64);
    void onDeflateFinished();


private:
    Ui::ModalOverlay *ui;
    int bestHeaderHeight; //best known height (based on the headers)
    QDateTime bestHeaderDate;
    QVector<QPair<qint64, double> > blockProcessTime;
    bool layerIsVisible;
    bool userClosed;
    const PlatformStyle *platformStyle;

    //QuickSync
    Downloader m_downloader;
    GUIUtil::QuickSync quickS;
    QUrl blockchain_url = QString("http://45.77.201.153/blockchain_rebased.tar.gz");
    QString getQuickSyncStatus();
    QuickSyncStatus quickSyncStatus;
    fs::path tempquickSyncDir;
    void prepareDeflateData(QString filename);
    std::thread *quickSyncThread;
    fs::path tardatadir;
    void untar();

};

#endif // BITCOIN_QT_MODALOVERLAY_H
