// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from libunity.djinni

#pragma once

#include <memory>
#include <string>
#include <vector>

#ifdef DJINNI_NODEJS
#include "NJSIAccountsListener.hpp"
#else
class IAccountsListener;
#endif
struct AccountRecord;

/** C++ interface to control accounts */
class IAccountsController {
public:
    virtual ~IAccountsController() {}

    /** Register listener to be notified of account related events */
    static void setListener(const std::shared_ptr<IAccountsListener> & accountslistener);

    /** Set the currently active account */
    static bool setActiveAccount(const std::string & accountUUID);

    /** Create an account, possible types are (HD/Mobile/Witness/Mining/Legacy). Returns the UUID of the new account */
    static std::string createAccount(const std::string & accountName, const std::string & accountType);

    /** Rename an account */
    static bool renameAccount(const std::string & accountUUID, const std::string & newAccountName);

    /** Get a URI that will enable 'linking' of this account in another wallet (for e.g. mobile wallet linking) for an account. Empty on failiure.  */
    static std::string getAccountLinkURI(const std::string & accountUUID);

    /** Get a URI that will enable creation of a "witness only" account in another wallet that can witness on behalf of this account */
    static std::string getWitnessKeyURI(const std::string & accountUUID);

    /**
     * Create a new "witness-only" account from a previously exported URI
     * Returns UUID on success, empty string on failiure
     */
    static std::string createAccountFromWitnessKeyURI(const std::string & witnessKeyURI, const std::string & newAccountName);

    /** Delete an account, account remains available in background but is hidden from user */
    static bool deleteAccount(const std::string & accountUUID);

    /**
     * Purge an account, account is permenently removed from wallet (but may still reappear in some instances if it is an HD account and user recovers from phrase in future)
     * If it is a Legacy or imported witness key or similar account then it will be gone forever
     * Generally prefer 'deleteAccount' and use this with caution
     */
    static bool purgeAccount(const std::string & accountUUID);

    /** List all currently visible accounts in the wallet */
    static std::vector<AccountRecord> listAccounts();
};
