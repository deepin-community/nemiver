//Author: Dodji Seketeli
/*
 *This file is part of the Nemiver project
 *
 *Nemiver is free software; you can redistribute
 *it and/or modify it under the terms of
 *the GNU General Public License as published by the
 *Free Software Foundation; either version 2,
 *or (at your option) any later version.
 *
 *Nemiver is distributed in the hope that it will
 *be useful, but WITHOUT ANY WARRANTY;
 *without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *See the GNU General Public License for more details.
 *
 *You should have received a copy of the
 *GNU General Public License along with Nemiver;
 *see the file COPYING.
 *If not, write to the Free Software Foundation,
 *Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *See COPYRIGHT file copyright information.
 */
#include "config.h"
#include <glib/gi18n.h>
#include <gtkmm/liststore.h>
#include "common/nmv-exception.h"
#include "nmv-expr-inspector-dialog.h"
#include "nmv-expr-inspector.h"
#include "nmv-ui-utils.h"

NEMIVER_BEGIN_NAMESPACE (nemiver)

class VariableHistoryStoreColumns : public Gtk::TreeModel::ColumnRecord
{
public:
    Gtk::TreeModelColumn<Glib::ustring> varname;
    VariableHistoryStoreColumns() { add (varname); }
};

VariableHistoryStoreColumns&
get_cols ()
{
    static VariableHistoryStoreColumns cols;
    return cols;
}

class ExprInspectorDialog::Priv {
    friend class ExprInspectorDialog;
    Gtk::ComboBox *var_name_entry;
    Glib::RefPtr<Gtk::ListStore> m_variable_history;
    Gtk::Button *inspect_button;
    Gtk::Button *add_to_monitor_button;
    SafePtr<ExprInspector> expr_inspector;
    Gtk::Dialog &dialog;
    Glib::RefPtr<Gtk::Builder> gtkbuilder;
    IDebugger &debugger;
    IPerspective &perspective;
    sigc::signal<void, IDebugger::VariableSafePtr> expr_monitoring_requested;
    // Functionality mask
    unsigned fun_mask;

    Priv ();
public:

    Priv (Gtk::Dialog &a_dialog,
          const Glib::RefPtr<Gtk::Builder> &a_gtkbuilder,
          IDebugger &a_debugger,
          IPerspective &a_perspective) :
        var_name_entry (0),
        inspect_button (0),
        dialog (a_dialog),
        gtkbuilder (a_gtkbuilder),
        debugger (a_debugger),
        perspective (a_perspective),
        fun_mask (FUNCTIONALITY_ALL)
    {
        LOG_FUNCTION_SCOPE_NORMAL_DD;
        build_dialog ();
        connect_to_widget_signals ();
    }

    /// Build the inspector dialog.
    ///
    /// Fetch widgets from gtkbuilder and initialize them.
    void
    build_dialog ()
    {
        LOG_FUNCTION_SCOPE_NORMAL_DD;

        var_name_entry =
            ui_utils::get_widget_from_gtkbuilder<Gtk::ComboBox>
                (gtkbuilder, "variablenameentry");
        m_variable_history =
            Gtk::ListStore::create (get_cols ());
        var_name_entry->set_model (m_variable_history);
        var_name_entry->set_entry_text_column (get_cols ().varname);

        inspect_button =
            ui_utils::get_widget_from_gtkbuilder<Gtk::Button> (gtkbuilder,
                                                          "inspectbutton");
        inspect_button->set_sensitive (false);

        add_to_monitor_button =
            ui_utils::get_widget_from_gtkbuilder<Gtk::Button> (gtkbuilder,
                                                               "addtomonitorbutton");
        add_to_monitor_button->set_sensitive (false);

        Gtk::Box *box =
            ui_utils::get_widget_from_gtkbuilder<Gtk::Box> (gtkbuilder,
                                                       "inspectorwidgetbox");

        expr_inspector.reset (new ExprInspector (debugger, perspective));
        expr_inspector->enable_contextual_menu (true);
        expr_inspector->cleared_signal ().connect
            (sigc::mem_fun
             (*this,
              &ExprInspectorDialog::Priv::on_variable_inspector_cleared));

        Gtk::ScrolledWindow *scr = Gtk::manage (new Gtk::ScrolledWindow);
        scr->set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        scr->set_shadow_type (Gtk::SHADOW_IN);
        scr->add (expr_inspector->widget ());
        box->pack_start (*scr);
        dialog.show_all ();
    }

    void
    connect_to_widget_signals ()
    {
        THROW_IF_FAIL (inspect_button);
        THROW_IF_FAIL (var_name_entry);
        inspect_button->signal_clicked ().connect (sigc::mem_fun
                (*this, &Priv::do_inspect_expression));
        add_to_monitor_button->signal_clicked ().connect
            (sigc::mem_fun (*this, &Priv::on_do_monitor_button_clicked));
        var_name_entry->signal_changed ().connect (sigc::mem_fun
                (*this, &Priv::on_var_name_changed_signal));
        var_name_entry->get_entry()->signal_activate ().connect (sigc::mem_fun
                (*this, &Priv::do_inspect_expression));
    }

    /// Inspect the variable (or, more generally the expression) which
    /// name the user has typed in the variable name entry, in the UI.
    void 
    do_inspect_expression ()
    {
        NEMIVER_TRY;

        THROW_IF_FAIL (var_name_entry);

        UString var_name = var_name_entry->get_entry ()->get_text ();
        if (var_name == "")
            return;

        inspect_expression (var_name, /*expand=*/true);

        NEMIVER_CATCH;
    }

    /// Inspect an expression.
    ///
    /// \param a_expr the expression to inspect.
    ///
    /// \param a_expand whether to expand the resulting expression
    /// tree.
    void
    inspect_expression (const UString& a_expr,
                      bool a_expand)
    {
        inspect_expression (a_expr, a_expand, 
                          sigc::mem_fun
                          (*this,
                           &ExprInspectorDialog::Priv::on_variable_inspected));
    }

    /// Inspect an expression.
    ///
    /// \param a_expr the expression to inspect.
    ///
    /// \param a_expand whether to expand the resulting expression
    /// tree.
    ///
    /// \param a_s a slot to invoke whenever the expresion has been
    /// inspected.
    void
    inspect_expression (const UString &a_expr,
                      bool a_expand,
                      const sigc::slot<void, 
                                       const IDebugger::VariableSafePtr> &a_s)
    {
        THROW_IF_FAIL (expr_inspector);
        THROW_IF_FAIL (m_variable_history);

        expr_inspector->inspect_expression
            (a_expr, a_expand, a_s);

        add_to_history (a_expr,
                        false /*append*/,
                        false /*don't allow duplicates*/);
    }

    /// Tests wheter if the variable expression exists in history.
    ///
    /// \param a_expr the expression to look for.
    ///
    /// \param a_iter if the return returned true and if this pointer
    /// is non-null, then *a_iter is filled with an iterator pointing
    /// at the expression found in history.
    /// 
    /// \return true if the variable expression a_expr exits in
    /// memory, false otherwise.
    bool
    exists_in_history (const UString &a_expr,
                       Gtk::TreeModel::iterator *a_iter = 0) const
    {
        THROW_IF_FAIL (m_variable_history);

        Gtk::TreeModel::iterator it;
        for (it = m_variable_history->children ().begin ();
             it != m_variable_history->children ().end ();
             ++it) {
            if ((*it)[get_cols ().varname] == a_expr) {
                if (a_iter != 0)
                    *a_iter = it;
                return true;
            }
        }
        return false;
    }

    /// Erases an expression from expression history.
    ///
    /// \param a_iter the iterator pointing to the expression to erase
    /// from history.
    void
    erase_expression_from_history (Gtk::TreeModel::iterator &a_iter)
    {
        THROW_IF_FAIL (m_variable_history);
        m_variable_history->erase (a_iter);
    }

    /// Add an expression to variable expression history.  If the
    /// expression already exists in history, it can either be
    /// duplicated or be not.  Also, the new expression can be either
    /// appended or prepended to history.
    ///
    /// \param a_expr the expression to add to history.
    ///
    /// \param a_prepend if true, prepend the expression to history.
    ///
    /// \param allow_dups if true, allow expressions to be present in
    /// more than copy in history.
    void
    add_to_history (const UString &a_expr,
                    bool a_prepend = false,
                    bool a_allow_dups = false)
    {
        // Don't append empty expressions.
        if (a_expr.empty ())
            return;

        // If the expression already exists in history, remove it, so
        // that it can be added again, as to be the latest added item
        // to historry.
        Gtk::TreeModel::iterator it;
        if (!a_allow_dups
            && exists_in_history (a_expr, &it))
            erase_expression_from_history (it);

        THROW_IF_FAIL (m_variable_history);
        if (a_prepend)
            it = m_variable_history->insert
                (m_variable_history->children ().begin ());
        else
            it = m_variable_history->append ();
        (*it)[get_cols ().varname] = a_expr;
    }

    void
    get_history (std::list<UString> &a_hist) const
    {
        Gtk::TreeModel::iterator it;
        for (it = m_variable_history->children ().begin ();
             it != m_variable_history->children ().end ();
             ++it) {
            Glib::ustring elem = (*it)[get_cols ().varname];
            a_hist.push_back (elem);
        }
    }

    /// Set the current history of variable expression to a new one.
    ///
    /// \param a_hist the new history to set.
    void
    set_history (const std::list<UString> &a_hist)
    {
        m_variable_history->clear ();
        std::list<UString>::const_iterator it;
        for (it = a_hist.begin (); it != a_hist.end (); ++it)
            add_to_history (*it, /*a_prepend=*/false,
                            /*a_allow_dups=*/false);
    }

    /// Set the functionality mask.  It's a mask made of the bits
    /// addressed by VarInspector::FunctionalityFlags.
    void
    functionality_mask (int a_mask)
    {
        fun_mask = a_mask;
    }

    /// Return the functionality mask.  It's a mask made of the bits
    /// addressed by VarInspector::FunctionalityFlags.
    unsigned
    functionality_mask ()
    {
        return fun_mask;
    }


    //************************
    //<signal handlers>
    //*************************

    /// Handler called when the name of the variable changes in the
    /// variable name entry field.
    void
    on_var_name_changed_signal ()
    {
        LOG_FUNCTION_SCOPE_NORMAL_DD;
        NEMIVER_TRY

        THROW_IF_FAIL (var_name_entry);
        THROW_IF_FAIL (inspect_button);

        UString var_name = var_name_entry->get_entry ()->get_text ();
        if (var_name == "")
            inspect_button->set_sensitive (false);
        else if (functionality_mask ()
                 & FUNCTIONALITY_EXPR_INSPECTOR)
            inspect_button->set_sensitive (true);

        // this handler is called when any text is changed in the entry or when
        // an item is selected from the combobox.  We don't want to inspect any
        // text that is typed into the entry, but we do want to inspect when
        // they choose an item from the dropdown list
        if (var_name_entry->get_active ()) {
            inspect_expression (var_name, true);
        }

        NEMIVER_CATCH
    }

    /// Handler called when the variable to be inspected is actually
    /// added to the inspector.
    void
    on_variable_inspected (const IDebugger::VariableSafePtr)
    {
        if ((functionality_mask () & FUNCTIONALITY_EXPR_MONITOR_PICKER))
            add_to_monitor_button->set_sensitive (true);
    }

    /// Handler called when the variable inspector is cleared.
    void
    on_variable_inspector_cleared ()
    {
        add_to_monitor_button->set_sensitive (false);
    }

    void
    on_do_monitor_button_clicked ()
    {
        NEMIVER_TRY;

        THROW_IF_FAIL (expr_inspector->get_expression ());

        expr_monitoring_requested.emit (expr_inspector->get_expression ());

        NEMIVER_CATCH
    }

    //************************
    //</signal handlers>
    //*************************
};//end class ExprInspectorDialog::Priv

/// The constructor of the ExprInspectorDilaog type.
///
/// \param a_parent the parent window of the dialog.
///
/// \param a_debugger the IDebugger interface to use to inspect the
/// expression.
///
/// \param a_perspective the IPerspective interface to use.
ExprInspectorDialog::ExprInspectorDialog (Gtk::Window &a_parent,
                                          IDebugger &a_debugger,
                                          IPerspective &a_perspective) :
    Dialog (a_perspective.plugin_path (),
            "exprinspectordialog.ui",
            "exprinspectordialog",
            a_parent)
{
    LOG_FUNCTION_SCOPE_NORMAL_DD;
    m_priv.reset
        (new ExprInspectorDialog::Priv (widget (),
                                       gtkbuilder (), a_debugger,
                                       a_perspective));
    THROW_IF_FAIL (m_priv);
}

ExprInspectorDialog::~ExprInspectorDialog ()
{
    LOG_D ("delete", "destructor-domain");
}

UString
ExprInspectorDialog::expression_name () const
{
    THROW_IF_FAIL (m_priv);
    THROW_IF_FAIL (m_priv->var_name_entry);
    return m_priv->var_name_entry->get_entry ()->get_text ();
}

/// Inspect an expression (including a variable)
///
/// \param a_var_name the expression to inspect.
void
ExprInspectorDialog::inspect_expression (const UString &a_var_name)
{
    THROW_IF_FAIL (m_priv);
    THROW_IF_FAIL (m_priv->var_name_entry);

    if (a_var_name != "") {
        m_priv->var_name_entry->get_entry ()->set_text (a_var_name);
        m_priv->inspect_expression (a_var_name, true);
    }
}

/// Inspect an expression (including a variable)
///
/// \param a_var_name the expression to inspect.
///
/// \param a_slot a slot to invoke whenever the expression has been
/// inspected.
void
ExprInspectorDialog::inspect_expression
(const UString &a_var_name,
 const sigc::slot<void, 
                  const IDebugger::VariableSafePtr> &a_slot)
{
    THROW_IF_FAIL (m_priv);
    THROW_IF_FAIL (m_priv->var_name_entry);

    if (a_var_name != "") {
        m_priv->var_name_entry->get_entry ()->set_text (a_var_name);
        m_priv->inspect_expression (a_var_name, true, a_slot);
    }
}

const IDebugger::VariableSafePtr
ExprInspectorDialog::expression () const
{
    THROW_IF_FAIL (m_priv);
    return m_priv->expr_inspector->get_expression ();
}

/// Return the variable inspector used by this dialog
ExprInspector& 
ExprInspectorDialog::inspector () const
{
    THROW_IF_FAIL (m_priv);
    return *m_priv->expr_inspector;
}

/// Set the history of variable expression to a new one.
///
/// \param a_hist the new history.
void
ExprInspectorDialog::set_history (const std::list<UString> &a_hist)
{
    THROW_IF_FAIL (m_priv);

    m_priv->set_history (a_hist);
}

void
ExprInspectorDialog::get_history (std::list<UString> &a_hist) const
{
    THROW_IF_FAIL (m_priv);
    m_priv->get_history (a_hist);
}

sigc::signal<void, IDebugger::VariableSafePtr>&
ExprInspectorDialog::expr_monitoring_requested ()
{
    THROW_IF_FAIL (m_priv);
    return m_priv->expr_monitoring_requested;
}

/// Set the functionality mask.  It's a mask made of the bits
/// addressed by ExprInspector::FunctionalityFlags.
void
ExprInspectorDialog::functionality_mask (int a_mask)
{
    THROW_IF_FAIL (m_priv);

    m_priv->functionality_mask (a_mask);
}

/// Return the functionality mask.  It's a mask made of the bits
/// addressed by ExprInspector::FunctionalityFlags.
unsigned
ExprInspectorDialog::functionality_mask ()
{
    THROW_IF_FAIL (m_priv);

    return m_priv->functionality_mask ();
}

NEMIVER_END_NAMESPACE (nemiver)

