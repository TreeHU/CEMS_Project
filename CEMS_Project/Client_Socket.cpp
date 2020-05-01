sprintf(SqlBuf, "DECLARE @cequip_Id varchar(500) Select Top 1 @cequip_Id = (select count(*) from Electricity_output_5min  where equip_Id = '%s' and  YYYYMMDDHH = '%s' and mmIndex = %d) if (@cequip_Id < 1) begin INSERT INTO Electricity_output_5min(equip_Id, YYYYMMDDHH, mmIndex, Value_5min, NowDate) VALUES('%s', '%s', '%d', %f, convert(varchar, getdate(), 121)) end", eid, day, Index, eid, day, Index, value);
Checked = false;
WriteDBLog((LPSTR)(LPCTSTR)("output ������"));
	}
	else {
	sprintf(SqlBuf, "DECLARE @cequip_Id varchar(500) Select Top 1 @cequip_Id = (select count(*) from Electricity_consumption_5min  where equip_Id = '%s' and  YYYYMMDDHH = '%s' and mmIndex = %d) if (@cequip_Id < 1) begin INSERT INTO Electricity_consumption_5min(equip_Id, YYYYMMDDHH, mmIndex, Value_5min, NowDate) VALUES('%s', '%s', '%d', %f, convert(varchar, getdate(), 121)) end", eid, day, Index, eid, day, Index, value);
	Checked = true;
	WriteDBLog((LPSTR)(LPCTSTR)("consumption ������"));
	}

	int ret = SQLExecDirect(hStmt, (SQLCHAR*)SqlBuf, SQL_NTS);

	//hkim -- proc ȣ�� �Լ� 
	ProcFc(eid, day, Index, value, Checked);



	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO) && (ret != SQL_NO_DATA))
	{

		CString index, Value;
		index.Format("%d", Index);
		Value.Format("%f", value);
		WriteDBLog((LPSTR)(LPCTSTR)("DB Insert ���� : qry = " + eid));
	}
	else
	{
		CString index, Value;
		index.Format("%d", Index);
		Value.Format("%f", value);
		//����
		//AfxMessageBox(_T("AfxMessageBox �׽�Ʈ"));
		WriteDBLog((LPSTR)(LPCTSTR)("DB Insert ���� : qry = " + eid + "," + day + "," + index + "," + Value));

	}


	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO) && (ret != SQL_NO_DATA)) { return 1; };
	return 0;
}


void ProcFc(CString eid, CString day, int Index, float value, bool Check) {
	//hkim �߰�
	// procedure call 


	SQLRETURN retCode;
	CString proclog = "";
	proclog.Format("%s, %d, %.5f, %s"
		, eid
		, Index
		, value
		, day
	);

	if (Check == false && Index % 3 == 0) {
		char sqlbuff[500];
		sprintf(sqlbuff, "{call CronProc_Output('%s',%d,'%s',%f)}", eid, Index, day, value);

		//WriteDBLog((LPSTR)(LPCTSTR)("stored procedure CronProc_Output �׽�Ʈ ����dkim"));
		int ret2 = SQLExecDirect(hStmt, (SQLCHAR*)sqlbuff, SQL_NTS);

		if ((ret2 != SQL_SUCCESS) && (ret2 != SQL_SUCCESS_WITH_INFO))
		{
			//WriteDBLog((LPSTR)(LPCTSTR)("stored procedure  CronProc_Output �׽�Ʈ ����"));
			WriteProcLog((LPSTR)(LPCTSTR)("OutPut_Procedure ����"));

		}
		else if (ret2 == SQL_SUCCESS_WITH_INFO)
		{
			WriteProcLog((LPSTR)(LPCTSTR)("Output_Procedure: " + proclog));
			//WriteDBLog((LPSTR)(LPCTSTR)("stored procedure  CronProc_Output �׽�Ʈ ����- SQL_SUCESS_WITH_INFO"));
		}
	}
	else if (Check == true && Index % 3 == 0) {
		char sqlbuff[500];
		sprintf(sqlbuff, "{call CronProc_ConSump('%s',%d,'%s',%f)}", eid, Index, day, value);
		//WriteDBLog((LPSTR)(LPCTSTR)("stored procedure CronProc_ConSump �׽�Ʈ ����dkim"));


		int ret2 = SQLExecDirect(hStmt, (SQLCHAR*)sqlbuff, SQL_NTS);

		if ((ret2 != SQL_SUCCESS) && (ret2 != SQL_SUCCESS_WITH_INFO))
		{
			WriteProcLog((LPSTR)(LPCTSTR)("ConSump_Procedure ����"));
			//WriteDBLog((LPSTR)(LPCTSTR)("stored procedure  CronProc_ConSump �׽�Ʈ ����"));
		}
		else if (ret2 == SQL_SUCCESS_WITH_INFO)
		{
			WriteProcLog((LPSTR)(LPCTSTR)("ConSump_Procedure: " + proclog));
			//WriteDBLog((LPSTR)(LPCTSTR)("stored procedure  CronProc_ConSump �׽�Ʈ ����- SQL_SUCESS_WITH_INFO"));
		}
	}
	retCode = SQLFreeStmt(hStmt, SQL_CLOSE);
	return;
}

