
# include               "AnswerHandler.hh"

# include               "babel.h"
# include               "ui_babel.h"
# include               "profile.h"
# include               "config.h"
# include               "connectdiag.h"
# include               "contactlist.h"
# include               "addcontact.h"
# include               "channelmanager.h"

AnswerHandler::         AnswerHandler()
{
    this->_hFunc[APacket::ID_AUTHENTIFICATION] = &AnswerHandler::handleAuthentificationAnswer;
    this->_hFunc[APacket::ID_REGISTER] = &AnswerHandler::handleRegisterAnswer;
    this->_hFunc[APacket::ID_ADD_CONTACT] = &AnswerHandler::handleAddContactAnswer;
    this->_hFunc[APacket::ID_UPDATE_USERNAME] = &AnswerHandler::handleUpdateUsernameAnswer;
    this->_hFunc[APacket::ID_UPDATE_STATUS] = &AnswerHandler::handleUpdateStatusAnswer;
    this->_hFunc[APacket::ID_SEARCH_USER] = &AnswerHandler::handleSearchUserAnswer;
    this->_hFunc[APacket::ID_CREATE_CHANNEL] = &AnswerHandler::handleCreateChannelAnswer;
    this->_hFunc[APacket::ID_ADD_USER_TO_CHANNEL] = &AnswerHandler::handleAddUserToChannelAnswer;
    this->_hFunc[APacket::ID_CALL] = &AnswerHandler::handleCallAnswer;


    this->_notifs[PacketAnswer::AUTH_ACCOUNT_NAME_NOT_FOUND] = std::make_pair(true, "Account Name not Found");
    this->_notifs[PacketAnswer::AUTH_BAD_PASSWORD] = std::make_pair(true, "Incorrect Password");
    this->_notifs[PacketAnswer::AUTH_ACCOUNT_BANNED] = std::make_pair(true, "Your account is banned");
    this->_notifs[PacketAnswer::AUTH_BAD_ACCOUNT_NAME] = std::make_pair(true, "Incorrect Account Name");
    this->_notifs[PacketAnswer::AUTH_FAILED] = std::make_pair(true, "Authentification Failed");

    this->_notifs[PacketAnswer::REGISTER_ACCOUNT_NAME_TAKEN] = std::make_pair(true, "Account Name Taken");
    this->_notifs[PacketAnswer::REGISTER_PASSWORD_INVALID] = std::make_pair(true, "Invalid Password");
    this->_notifs[PacketAnswer::REGISTER_ACCOUNT_NAME_INVALID] = std::make_pair(true, "Invalid Account Name");
    this->_notifs[PacketAnswer::REGISTER_ACCOUNT_CREATED] = std::make_pair(false, "Account created");

    this->_notifs[PacketAnswer::ADD_CONTACT_USER_ID_NOT_FOUND] = std::make_pair(true, "User not found");
    this->_notifs[PacketAnswer::ADD_CONTACT_REQUEST_ALREADY_SEND] = std::make_pair(true, "Request already sent");
    this->_notifs[PacketAnswer::ADD_CONTACT_ALREADY_FRIEND] = std::make_pair(true, "User is already a contact");
    this->_notifs[PacketAnswer::ADD_CONTACT_SUCCESS_WAITING] = std::make_pair(false, "Request waiting for validation from the other user");
    this->_notifs[PacketAnswer::ADD_CONTACT_SUCCESS_FRIEND] = std::make_pair(false, "The other user already added you, you accept his invitation");

    this->_notifs[PacketAnswer::UPDATE_USERNAME_INVALID] = std::make_pair(true, "Invalid Username");
    this->_notifs[PacketAnswer::UPDATE_USERNAME_SUCCESS] = std::make_pair(false, "Username changed");

    this->_notifs[PacketAnswer::UPDATE_STATUS_INVALID] = std::make_pair(true, "Invalid Status");
    this->_notifs[PacketAnswer::UPDATE_STATUS_SUCCESS] = std::make_pair(false, "Status changed");

    this->_notifs[PacketAnswer::SEARCH_USER_INVALID_PATTERN] = std::make_pair(true, "Invalid Pattern");
    this->_notifs[PacketAnswer::SEARCH_USER_NOT_FOUND] = std::make_pair(true, "No user found");

    this->_notifs[PacketAnswer::CREATE_CHANNEL_USERS_DOESNT_EXISTS] = std::make_pair(true, "One / several user doesn't exists");
    this->_notifs[PacketAnswer::CREATE_CHANNEL_USERS_NOT_CONNECTED] = std::make_pair(true, "One / several user isn't connected");
    this->_notifs[PacketAnswer::CREATE_CHANNEL_ALREADY_EXISTS] = std::make_pair(true, "A similar channel already exist");

    this->_notifs[PacketAnswer::ADD_USER_TO_CHANNEL_CHANID_DOESNT_EXISTS] = std::make_pair(true, "The channel doesn't exist");
    this->_notifs[PacketAnswer::ADD_USER_TO_CHANNEL_USER_DOESNT_EXISTS] = std::make_pair(true, "The user doesn't exist");
    this->_notifs[PacketAnswer::ADD_USER_TO_CHANNEL_PERMISSION_DENIED] = std::make_pair(true, "You don't have the rights to add a new user in this channel (not in the channel)");
    this->_notifs[PacketAnswer::ADD_USER_TO_CHANNEL_USER_NOT_IN_CONTACT_LIST] = std::make_pair(true, "The user isn’t in your contact list");
    this->_notifs[PacketAnswer::ADD_USER_TO_CHANNEL_SUCCESS] = std::make_pair(false, "User successfully added to the channel");
}

AnswerHandler::         ~AnswerHandler()
{

}

void                    AnswerHandler:: handle(APacket::packetID id, PacketAnswer::AnswerCode code) const
{
    auto it = this->_hFunc.find(id);

    if (it != this->_hFunc.end())
        (this->*(it->second))(code);
}

void                    AnswerHandler:: handleAuthentificationAnswer(PacketAnswer::AnswerCode code) const
{
    auto it = this->_notifs.find(code);

    if (it != this->_notifs.end())
        bab->getCoDiag()->notify(it->second.first, it->second.second);
}

void                    AnswerHandler:: handleRegisterAnswer(PacketAnswer::AnswerCode code) const
{
    auto it = this->_notifs.find(code);

    if (it != this->_notifs.end())
        bab->getCoDiag()->notify(it->second.first, it->second.second);
}

void                    AnswerHandler:: handleAddContactAnswer(PacketAnswer::AnswerCode code) const
{
    auto it = this->_notifs.find(code);

    if (it != this->_notifs.end())
        bab->getAddContact()->notify(it->second.first, it->second.second);
}

void                    AnswerHandler:: handleUpdateUsernameAnswer(PacketAnswer::AnswerCode code) const
{
    auto it = this->_notifs.find(code);

    if (it != this->_notifs.end())
        bab->getConfig()->notify(it->second.first, it->second.second);
}

void                    AnswerHandler:: handleUpdateStatusAnswer(PacketAnswer::AnswerCode code) const
{
    auto it = this->_notifs.find(code);

    if (it != this->_notifs.end())
        bab->getConfig()->notify(it->second.first, it->second.second);
}

void                    AnswerHandler:: handleSearchUserAnswer(PacketAnswer::AnswerCode code) const
{
    auto it = this->_notifs.find(code);

    if (it != this->_notifs.end())
        bab->getAddContact()->notify(it->second.first, it->second.second);
}

void                    AnswerHandler:: handleCreateChannelAnswer(PacketAnswer::AnswerCode code) const
{
    auto it = this->_notifs.find(code);

    if (it != this->_notifs.end())
        bab->getContactList()->notify(it->second.first, it->second.second);
}

void                    AnswerHandler:: handleAddUserToChannelAnswer(PacketAnswer::AnswerCode code) const
{
    auto it = this->_notifs.find(code);

    if (it != this->_notifs.end())
        bab->getContactList()->notify(it->second.first, it->second.second);
}

void                    AnswerHandler:: handleCallAnswer(PacketAnswer::AnswerCode) const
{
    // auto it = this->_notifs.find(code);

    // if (it != this->_notifs.end())
    //     bab->getCoDiag()->notify(it->second.first, it->second.second);
}