// Copyright (c) 2018-2019 The Gulden developers
// Authored by: Malcolm MacLeod (mmacleod@gmx.com)
// Distributed under the GULDEN software license, see the accompanying
// file COPYING

package com.gulden.unity_wallet.main_activity_fragments

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.gulden.unity_wallet.Currencies
import com.gulden.unity_wallet.R
import com.gulden.unity_wallet.WalletActivity
import com.gulden.unity_wallet.ui.LocalCurrenciesAdapter
import kotlinx.android.synthetic.main.fragment_local_currencies.*


class LocalCurrencyFragment : androidx.fragment.app.Fragment() {

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View?
    {
        return inflater.inflate(R.layout.fragment_local_currencies, container, false)
    }

    override fun onResume()
    {
        super.onResume()
        (activity as WalletActivity).showSettingsTitle("Select currency")
    }

    override fun onStop()
    {
        super.onStop()
        (activity as WalletActivity).hideSettingsTitle()
    }

    override fun onActivityCreated(savedInstanceState: Bundle?)
    {
        super.onActivityCreated(savedInstanceState)

        val adapter = LocalCurrenciesAdapter(this.context!!, Currencies.knownCurrencies)
        currenciesList.adapter = adapter
        currenciesList.setOnItemClickListener { parent, _, position, _ ->
            adapter.setSelectedPosition(position)
            adapter.notifyDataSetChanged()
        }
    }

}