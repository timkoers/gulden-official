// Copyright (c) 2016-2019 The Gulden developers
// Authored by: Willem de Jonge (willem@isnapp.nl)
// Distributed under the GULDEN software license, see the accompanying
// file COPYING

#include "extendwitnessdialog.h"
#include <qt/_Gulden/forms/ui_extendwitnessdialog.h>

#include <stdexcept>
#include "wallet/wallet.h"
#include "gui.h"
#include "wallet/witness_operations.h"

#define LOG_QT_METHOD LogPrint(BCLog::QT, "%s\n", __PRETTY_FUNCTION__)

ExtendWitnessDialog::ExtendWitnessDialog(const QStyle *_platformStyle, QWidget *parent)
: QFrame( parent )
, ui( new Ui::ExtendWitnessDialog )
, platformStyle( _platformStyle )
{
    ui->setupUi(this);

    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(ui->extendButton, SIGNAL(clicked()), this, SLOT(extendClicked()));
}

ExtendWitnessDialog::~ExtendWitnessDialog()
{
    delete ui;
}

void ExtendWitnessDialog::cancelClicked()
{
    LOG_QT_METHOD;

    Q_EMIT dismiss(this);
}

void ExtendWitnessDialog::extendClicked()
{
    LOG_QT_METHOD;

    // TODO: get required parameters, ie. lock duration, funding account

    if(QDialog::Accepted == GUI::createDialog(this, "Confirm extending", tr("Extend"), tr("Cancel"), 600, 360, "ExtendWitnessConfirmationDialog")->exec())
    {
        pactiveWallet->BeginUnlocked(_("Wallet unlock required to extend witness"), [=](){

            try {
                // TODO: fill actual parameters
                extendwitnessaccount(pactiveWallet,
                                     nullptr, // CAccount* fundingAccount,
                                     nullptr, // CAccount* witnessAccount,
                                     0, // CAmount amount,
                                     0, // uint64_t requestedLockPeriodInBlocks,
                                     nullptr, nullptr); // ignore result params

                // request dismissal only when succesful
                Q_EMIT dismiss(this);

            } catch (std::runtime_error& e) {
                GUI::createDialog(this, e.what(), tr("Okay"), QString(""), 400, 180)->exec();
            }

            pactiveWallet->EndUnlocked();
        });
    }
}
