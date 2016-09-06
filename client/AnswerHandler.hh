
# ifndef                ANSWER_HANDLER_HH_
# define                ANSWER_HANDLER_HH_

# include               <string>
# include               <utility>
# include               <map>
# include               "PacketAnswer.hh"

class                   AnswerHandler
{
public:
    AnswerHandler();
    ~AnswerHandler();

private:
    AnswerHandler(AnswerHandler const &) = delete;
    AnswerHandler&      operator=(AnswerHandler const &) = delete;

private:
    typedef void        (AnswerHandler::*funcPtr)(PacketAnswer::AnswerCode) const;
    std::map<APacket::packetID, funcPtr> _hFunc;
    std::map<PacketAnswer::AnswerCode, std::pair<bool, std::string> >  _notifs;

public:
    void                handle(APacket::packetID, PacketAnswer::AnswerCode) const;

public:
    void                handleAuthentificationAnswer(PacketAnswer::AnswerCode) const;
    void                handleRegisterAnswer(PacketAnswer::AnswerCode) const;
    void                handleAddContactAnswer(PacketAnswer::AnswerCode) const;
    void                handleUpdateUsernameAnswer(PacketAnswer::AnswerCode) const;
    void                handleUpdateStatusAnswer(PacketAnswer::AnswerCode) const;
    void                handleSearchUserAnswer(PacketAnswer::AnswerCode) const;
    void                handleCreateChannelAnswer(PacketAnswer::AnswerCode) const;
    void                handleAddUserToChannelAnswer(PacketAnswer::AnswerCode) const;
    void                handleSendMessageAnswer(PacketAnswer::AnswerCode) const;
    void                handleCallAnswer(PacketAnswer::AnswerCode) const;
};

# endif                 /* !ANSWER_HANDLER_HH */