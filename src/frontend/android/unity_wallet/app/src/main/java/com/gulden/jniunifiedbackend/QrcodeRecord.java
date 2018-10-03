// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from libunity.djinni

package com.gulden.jniunifiedbackend;

/**
 * Copyright (c) 2018 The Gulden developers
 * Authored by: Malcolm MacLeod (mmacleod@webmail.co.za)
 * Distributed under the GULDEN software license, see the accompanying
 * file COPYING
 */
public final class QrcodeRecord {


    /*package*/ final int mWidth;

    /*package*/ final byte[] mPixelData;

    public QrcodeRecord(
            int width,
            byte[] pixelData) {
        this.mWidth = width;
        this.mPixelData = pixelData;
    }

    public int getWidth() {
        return mWidth;
    }

    public byte[] getPixelData() {
        return mPixelData;
    }

    @Override
    public String toString() {
        return "QrcodeRecord{" +
                "mWidth=" + mWidth +
                "," + "mPixelData=" + mPixelData +
        "}";
    }

}
