// Copyright (c) 2016-2018 The Gulden developers
// Authored by: Malcolm MacLeod (mmacleod@webmail.co.za)
// Distributed under the GULDEN software license, see the accompanying
// file COPYING

#include "backupdialog.h"
#include <qt/_Gulden/forms/ui_backupdialog.h>

#include "wallet/wallet.h"
#include "walletmodel.h"
#include "guiconstants.h"
#include "unity/appmanager.h"



BackupDialog::BackupDialog(const QStyle* _platformStyle, QWidget* parent, WalletModel* model)
: QFrame( parent )
, ui( new Ui::BackupDialog )
, platformStyle( _platformStyle )
, walletModel(model)
{
    ui->setupUi(this);


    ui->labelBackupPhrase->setVisible(false);
    ui->labelShowBackupPhrase->setVisible(true);

    ui->labelShowBackupPhrase->setCursor(Qt::PointingHandCursor);
    ui->buttonShowBackupPhrase->setCursor(Qt::PointingHandCursor);
    ui->buttonSaveToFile->setCursor(Qt::PointingHandCursor);
    ui->buttonDone->setCursor(Qt::PointingHandCursor);

    ui->frameBackupDialogsButtons->setVisible(true);

    connect(ui->labelShowBackupPhrase, SIGNAL(clicked()), this, SLOT(showBackupPhrase()));
    connect(ui->buttonShowBackupPhrase, SIGNAL(clicked()), this, SLOT(showBackupPhrase()));
    connect(ui->buttonSaveToFile, SIGNAL(clicked()), this, SIGNAL(saveBackupFile()));
    connect(ui->buttonDone, SIGNAL(clicked()), this, SIGNAL(dismiss()));
}

void BackupDialog::showBackupPhrase()
{
    ui->labelBackupPhrase->setVisible(true);
    ui->labelShowBackupPhrase->setVisible(false);
    ui->buttonShowBackupPhrase->setVisible(false);

    LOCK(pactiveWallet->cs_wallet);
    WalletModel::UnlockContext ctx(walletModel->requestUnlock());
    if (ctx.isValid())
    {
        int64_t birthTime = 0;

        // determine block time of earliest transaction (if any)
        // if this cannot be determined for every transaction a phrase without birth time acceleration will be used
        int64_t firstTransactionTime = std::numeric_limits<int64_t>::max();
        for (CWallet::TxItems::const_iterator it = pactiveWallet->wtxOrdered.begin(); it != pactiveWallet->wtxOrdered.end(); ++it)
        {
            CWalletTx* wtx = it->second.first;
            if (!wtx->hashUnset())
            {
                CBlockIndex* index = mapBlockIndex[wtx->hashBlock];
                if (index && index->IsValid(BLOCK_VALID_HEADER))
                    firstTransactionTime = std::min(firstTransactionTime, std::max(int64_t(0), index->GetBlockTime()));
                else
                {
                    firstTransactionTime = 0;
                    break;
                }
            }
        }

        if (firstTransactionTime < std::numeric_limits<int64_t>::max())
            birthTime = firstTransactionTime;

        std::set<SecureString> allPhrases;
        for (const auto& seedIter : pactiveWallet->mapSeeds)
        {
            SecureString phrase = GuldenAppManager::composeRecoveryPhrase(seedIter.second->getMnemonic(), birthTime);
            allPhrases.insert(phrase);
        }

        if (allPhrases.size() == 1)
        {
            ui->labelBackupPhraseHeading->setText(tr("Below is your secret recovery phrase, write it down and keep it safe. Lose phrase = lose Guldens. Someone else with access to the phrase = lose Guldens."));
            ui->labelBackupPhrase->setText(QString::fromStdString(allPhrases.begin()->c_str()));
        }
        else if (allPhrases.size() > 1)
        {
            ui->labelBackupPhraseHeading->setText(tr("Below are your secret recovery phrases, write them down and keep them safe. Lose phrase = lose Guldens. Someone else with access to the phrase = lose Guldens."));
            QString phrases;
            for (const auto& phrase : allPhrases)
            {
                phrases += QString::fromStdString((phrase + "\n").c_str());
            }
            ui->labelBackupPhrase->setText(phrases);
        }
        else
        {
            ui->labelBackupPhraseHeading->setText(tr("No recovery phrases present for this wallet."));
        }
    }

    bool haveNonHDAccounts = false;
    for (const auto& accountIter : pactiveWallet->mapAccounts)
    {
        if (!accountIter.second->IsHD())
        {
            haveNonHDAccounts = true;
        }
    }

    if (haveNonHDAccounts)
    {
        ui->labelSaveToDiskDescription->setText(tr("Your wallet contains some legacy non-HD accounts, these cannot be restored using recovery phrases and require regular disk backup. Please ensure you make regular disk backups of your wallet, or delete the legacy account in order to ensure your funds remain secured."));
        ui->labelSaveToDiskDescription->setStyleSheet(STYLE_INVALID);
    }
}

BackupDialog::~BackupDialog()
{
    //fixme: (2.1) - Burn visible phrase.
    delete ui;
}
