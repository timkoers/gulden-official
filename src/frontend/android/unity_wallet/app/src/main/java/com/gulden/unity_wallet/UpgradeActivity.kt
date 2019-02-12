// Copyright (c) 2018 The Gulden developers
// Authored by: Willem de Jonge (willem@isnapp.nl)
// Distributed under the GULDEN software license, see the accompanying
// file COPYING

package com.gulden.unity_wallet

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.text.TextUtils
import android.util.Log
import android.view.View
import android.view.inputmethod.InputMethodManager
import androidx.appcompat.app.AppCompatActivity
import com.gulden.jniunifiedbackend.GuldenUnifiedBackend
import com.gulden.unity_wallet.Constants.OLD_WALLET_PROTOBUF_FILENAME
import kotlinx.android.synthetic.main.upgrade_password.view.*
import org.guldenj.wallet.Wallet
import org.guldenj.wallet.WalletProtobufSerializer
import org.jetbrains.anko.alert
import org.jetbrains.anko.appcompat.v7.Appcompat
import org.jetbrains.anko.design.longSnackbar
import java.io.File
import java.io.FileInputStream

private const val TAG = "upgrade-activity"

class UpgradeActivity : AppCompatActivity(), UnityCore.Observer
{
    private val coreObserverProxy = CoreObserverProxy(this, this)
    private var wallet: Wallet? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_upgrade)

        UnityCore.instance.addObserver(coreObserverProxy)

        wallet = walletFromProtobufFile(getFileStreamPath(OLD_WALLET_PROTOBUF_FILENAME))
    }

    override fun onDestroy() {
        super.onDestroy()

        UnityCore.instance.removeObserver(coreObserverProxy)
    }

    override fun onCoreReady(): Boolean {
        gotoActivity(WalletActivity::class.java)
        return true
    }

    fun onUpgrade(view: View)
    {
        wallet?.let { mywallet ->
            if (mywallet.isEncrypted) {
                val myCustomView = layoutInflater.inflate(R.layout.upgrade_password, null)
                val builder = alert(Appcompat) {
                    title = getString(R.string.upgrade_enter_password)
                    customView = myCustomView
                    positiveButton("OK") {
                        try {
                            mywallet.decrypt(myCustomView.password.text)
                            extractAndInitRecovery(mywallet)
                        }
                        catch (exception: Throwable) {
                            view.longSnackbar(getString(R.string.upgrade_wrong_password).format(exception.message!!))
                        }
                    }
                }
                val dialog = builder.build()
                dialog.setOnShowListener {
                    myCustomView.password.requestFocus()
                    val imm = application.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
                    imm.showSoftInput(myCustomView.password, InputMethodManager.SHOW_IMPLICIT)
                }
                dialog.show()
            }
            else
                extractAndInitRecovery(mywallet)
        }
    }

    private fun extractAndInitRecovery(wallet: Wallet) {
        assert(!wallet.isEncrypted)
        val recoveryPhrase = wallet.keyChainSeed.mnemonicCode?.joinToString(" ")
        assert(GuldenUnifiedBackend.IsValidRecoveryPhrase(recoveryPhrase))
        Log.i(TAG, "old wallet mnemonic extracted")
        GuldenUnifiedBackend.InitWalletFromRecoveryPhrase(recoveryPhrase)
    }

    private fun walletFromProtobufFile(walletFile: File): Wallet {
        Log.i(TAG, "start loading old protobuf wallet")
        assert(walletFile.exists()) // require existence has already been established
        val walletStream = FileInputStream(walletFile)
        val wallet = WalletProtobufSerializer().readWallet(walletStream)
        Log.i(TAG, "finished loading protobuf wallet")
        return wallet
    }

    private fun gotoActivity(cls: Class<*> )
    {
            val intent = Intent(this, cls)
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK)
            startActivity(intent)
            finish()
    }
}
