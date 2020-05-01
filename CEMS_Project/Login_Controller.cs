using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Data;


namespace Web.Controllers
{
    public class LoginController : WebController
    {
        // Login/Index : �α��� ȭ��                
        public ActionResult Index(string id)
        {
            this.ClearUserInfo();
            if (id == null) id = this.UserID;



            ViewBag.ID = id;

            return View("Index");
        }

        public ActionResult Signup()
        {
            return View();
        }

        public ActionResult SignupEvent()
        {

            string id = Request.Form["SignupID"];
            string pwd = Request.Form["SignupPassword"];

            Biz_Common.BizUser biz = new Biz_Common.BizUser();
            biz.SP_USER_Signup(id, pwd);


            return Redirect("~/Login/index");

        }


        // Login/Account : �α��� �õ�           
        public ActionResult Account()
        {
            string id = Request.Form["ID"];
            string pwd = Request.Form["Password"];

            Biz_Common.BizUser biz = new Biz_Common.BizUser();
            DataTable dt = biz.SP_USER_LOGIN(id, pwd);

            // �α��� ����
            if (dt.Rows.Count == 1)
            {
                this.Session["USER"] = dt.Rows[0]["USERID"];
                this.Session["USERNAME"] = dt.Rows[0]["USERNAME"];
                this.FillUserInfo(dt);

                //dt.Dispose();

                if (dt.Rows[0]["USERID"].ToString() == "admin")
                {
                    return Redirect("~/Home/Index");

                }
                else
                {
                    return Redirect("~/IndividualSite/Index");
                }





            }

            // �α��� ����
            else
            {
                dt.Dispose();
                return Index(id);
            }
        }

    }
}