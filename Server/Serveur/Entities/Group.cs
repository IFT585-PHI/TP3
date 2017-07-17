using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    class Group : Entity
    {
        String _name;
        String _description;
        Admin _admin;

        List<int> _members; //int = userID
        List<int> _pendingInvitations; //int = userId;
        Dictionary<int, int> _filesVersion; //int = fileID, int = fileVersion

        public Group(int id, string name, string description, int userId)
            :base(id)
        {
            _name = name;
            _description = description;
            _admin = new Admin(id, userId);
        }

        public Group(int id, string name, string description, Admin admin, List<int> members, Dictionary<int, int> filesVersion)
            : base(id)
        {
            _name = name;
            _description = description;
            _admin = admin;
            _members = members;
            _filesVersion = filesVersion;
        }

        public bool AddMember(int userId)
        {
            if (DoesMemberExists(userId))
                return false; ;

            _members.Add(userId);
            return true;
        }

        public bool RemoveMember(int userId)
        {
            if (!DoesMemberExists(userId))
                return false; ;

            _members.Remove(userId);
            return true;
        }

        public bool AddFile(int fileId)
        {
            if (DoesFileExists(fileId))
                return false;

            _filesVersion.Add(fileId, 1);
            return true;
        }

        public bool UpdateFile(int fileId)
        {
            if (!DoesFileExists(fileId))
                return false;

            _filesVersion[fileId] += 1;
            return true;
        }

        public bool RemoveFile(int fileId)
        {
            if (!DoesFileExists(fileId))
                return false;

            _filesVersion.Remove(fileId);
            return true;
        }

        private bool DoesMemberExists(int userId)
        {
            return _members.Contains(userId);
        }

        private bool DoesFileExists(int fileId)
        {
            return _filesVersion.Keys.Contains(fileId);
        }

        public void SetAdmin(int userId)
        {
            _admin.SetUserId(userId);
        }

        public bool AddPendingInvitation(int userId)
        {
            if (DoesPendingInvitationExists(userId))
                return false;

            _pendingInvitations.Add(userId);
            return true;
        }

        public bool RemovePendingInvitation(int userId)
        {
            if (!DoesPendingInvitationExists(userId))
                return false;

            _pendingInvitations.Remove(userId);
            return true;
        }

        private bool DoesPendingInvitationExists(int userId)
        {
            return _pendingInvitations.Contains(userId);
        }
    }
}
