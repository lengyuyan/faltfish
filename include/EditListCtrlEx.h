#pragma once
#ifndef _INCLUDE_CEDITLISTCTRLEX_H_
#define _INCLUDE_CEDITLISTCTRLEX_H_
#include <vector>

using std::vector;


#define EDITLISTCTRLEX_VERSION "editlistctrlex_1.1.1.20170122_beta"


//����2���ӿ�
/*
��Ҫ�����û��жϱ༭���������Ƿ�Ϸ������ֶ����Զ����µ�list��
��Ҫ�ֶ����£�����Ҫ�ڴ������н��� MLSM_ITEMCHANGED��Ϣ,
���ҽ�����Ϣʱ�ͱ��봦�������Ƿ���£�������ܳ���(ɾ���л�����Ӻ�����)
��������:
1. ���� afx_msg LRESULT OnMsgChangeItem(WPARAM wParam, LPARAM lParam);
2. ʵ�� LRESULT CxxDlg::OnMsgChangeItem(WPARAM wParam, LPARAM lParam)
{
    int row = wParam & 0xFF;
    int col = wParam>>16;
}

3. �����Ϣӳ�� ON_MESSAGE(MLSM_ITEMCHANGED, &CxxDlg::OnMsgChangeItem)
*/

//����combox�ؼ�Ƕ�룬����5���ӿ�
/* ������combox���ݣ���������˳��洢�磺Client;Server;����һ�飬����2��
ѡ��Client/Server,combox Ϊdropdownģʽ�������Լ����룬ֻ��ѡ��
����������1�����ýӿڣ�2����ȡ�ӿ�

��ͨ��SetComboxCell��SetEditCell������ĳ����Ԫ��������ֿؼ����룬������
row == -1ʱ����ʾĳ��ȫ�����øÿؼ����룬colͬ��
combox �ı���Ϣӳ������Ϊ MLSM_ITEMCHANGEDCOMBOX ��Ҫ�������ݸı���Ϣ����
��Ҫ�����Ϣӳ�� ON_MESSAGE(MLSM_ITEMCHANGEDCOMBOX, &CxxDlg::OnMsgChangeItem)*/


#define IDC_LIST_EDITBOX    0xfff1
#define IDC_LIST_COMBOX     0xfff2
#define MLSM_ITEMCHANGEDEX  (WM_USER + 201)

// CEditListCtrlEx
class CMEdit;
class CMComboBox;
class _declspec(dllexport) CEditListCtrlEx : public CListCtrl
{
    DECLARE_DYNAMIC(CEditListCtrlEx)

public:
    CEditListCtrlEx();
    virtual ~CEditListCtrlEx();

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSelchangeCombox();

    afx_msg LRESULT OnMsgKillFocus(WPARAM wParam, LPARAM lParam);

    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

    BOOL DeleteAllItems();
    BOOL DeleteItem(_In_ int nItem);

    //�ֶ����±༭��ֵ��list
    void UpdateItemText();

    //����item����ģʽ��Ĭ��Ϊ�Զ�����
    void SetUpdateItemModel(BOOL bAutoUpdate = TRUE);

    //��ȡ�����ĵ�ֵ���ֶ�ģʽ�²��ܻ�ȡ��
    CString GetEditText() { return m_edtStr; }

    CString GetCurItemText();

    //index ��ţ���0��ʼ,��˳��洢
    int AddComboxContent(LPCTSTR pszContent);    //����combox���ݣ��Էֺŷָ�
    int GetComboxContent(int index, LPTSTR pszContent);
    CString GetComboxContent(int index);

    //����ĳ����Ԫ��Ϊcomboxѡ��, index combox��ѡ��������
    //row = -1ʱ��ʾĳ�����У� col= -1ʱ,��ʾĳ������
    int SetComboxCell(int row = 0xffff, int col = 0xffff, int index = 0);
    int SetEditCell(int row = 0xffff, int col = 0xffff);    //����ĳ����Ԫ��ɱ༭
    void EraseEditCell(int row = 0xffff, int col = 0xffff);  //ɾ���ɱ༭��Ԫ�񣬰�����combox��

protected:
    DECLARE_MESSAGE_MAP()

    void SetTextToList();
    void SetContentToBox(CString str);

private:

    int         m_Row;
    int         m_Col;
    int         m_selectRow;
    int         m_selectCol;
    CMEdit*     m_pEditItem;
    CMComboBox* m_pEditCom;

    BOOL        m_bAutoUpdate;  //�Ƿ��Զ�����
    CString     m_edtStr;       //�༭��ֵ

    vector<CString>     m_vecComContent;    //����
    vector<int>         m_vecEditCell;      //edit��Ԫ��
    vector<int>         m_vecComCell;       //com��Ԫ��
    vector<int>         m_vecComIndex;      //com��Ԫ���Ӧ����index
};

#endif  //�ظ�����