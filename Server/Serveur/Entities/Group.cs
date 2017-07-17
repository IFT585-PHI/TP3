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
            _members = new List<int>();
            _pendingInvitations = new List<int>();
            _filesVersion = new Dictionary<int, int>();
        }

        public Group(int id, string name, string description, Admin admin, List<int> members, List<int> pendingInvitations, Dictionary<int, int> filesVersion)
            : base(id)
        {
            _name = name;
            _description = description;
            _admin = admin;
            _members = members;
            _pendingInvitations = pendingInvitations;
            _filesVersion = filesVersion;
        }

        public bool AddMember(int userId)
        {
            if (DoesMemberExists(userId) || !DoesPendingInvitationExists(userId))
                return false;

            _members.Add(userId);
            _pendingInvitations.Remove(userId);
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

            //do something with file content?
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

        private bool DoesMemberExists(int userId)
        {
            return _members.Contains(userId);
        }

        private bool DoesFileExists(int fileId)
        {
            return _filesVersion.Keys.Contains(fileId);
        }

        private bool DoesPendingInvitationExists(int userId)
        {
            return _pendingInvitations.Contains(userId);
        }
        public void SetAdmin(int userId)
        {
            _admin.SetUserId(userId);
        }
    }
}
