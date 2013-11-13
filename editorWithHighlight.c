/* 
 *	Text editor project
 *
 *	Syntax Terror
 */



#include <gtk/gtk.h>
#include <stdio.h>

/* List of functions and their corresponding tool tips. */
static char *tips[][2] = 
  {
    {"printf", "(const char *format, ...)"},
    {"fprintf", "(FILE *stream, const char *format, ...)"},
    {"scanf", "(const char *format, ...)"},
    {"snprintf", "(char *str, size_t size, const char *format, ...)"},
    {"fputs", "(const char *s, FILE *stream)"},
    {"putc", "(int c, FILE *stream)"},
    {"putchar", "(int c)"},
    {"puts", "(const char *s)"},
		{"for","(initialisation; ending condition; counter)"},  
		{"if", "(condition)"},
		{"else if","(condition)"},
		{"while","(condition)"},
	};

#define NUM_TIPS (sizeof (tips) / sizeof (tips[0]))

typedef struct
{
  GtkWidget *textview, *search;
} TextEditor;

GtkTextTagTable *table1;
GtkTextTag *highlight_tag;

void search_button_clicked (GtkWidget *search_button, TextEditor *editor)
{
	if(GTK_IS_TEXT_VIEW (editor->textview)&&!GTK_IS_TEXT_TAG_TABLE (table1)){  
	const gchar *text;
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  GtkTextIter mstart, mend; 
  gboolean found;
	//gchar text_highlight;	
	

  text = gtk_entry_get_text (GTK_ENTRY (editor->search));

  /* Get the buffer associated with the text view widget. */
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
  /* Search from the start from buffer for text. */
  
	gtk_text_buffer_get_start_iter (buffer, &iter);
  found = gtk_text_iter_forward_search (&iter, text, 0, &mstart, &mend, NULL);
	
	if(GTK_IS_TEXT_TAG_TABLE (table1))
		gtk_text_tag_table_remove(table1,highlight_tag);

	highlight_tag = gtk_text_buffer_create_tag(buffer, NULL,"foreground", "blue", NULL);	
	
	if(GTK_IS_TEXT_TAG_TABLE (table1))
		gtk_text_tag_table_add(table1, highlight_tag);

	while(found == 1){
	
	found = gtk_text_iter_forward_search (&iter, text, 0, &mstart, &mend, NULL);

  if (found){
      /* If found, highlight the text. */
  //  gtk_text_buffer_select_range (buffer, &mstart, &mend);
	gtk_text_buffer_apply_tag(buffer, highlight_tag, &mstart, &mend);	
	iter = mend;
	}
	}
	}
}

void font_highlight ( TextEditor *editor)
{

	g_print("enter");  
	 gchar *text;
  GtkTextBuffer *buffer;
  GtkTextIter iter;
  GtkTextIter mstart, mend; 
  gboolean found;
	//gchar text_highlight;	
 	GtkTextTag *highlight_tag2;	
  text = "for";

	//scanf("%s",text);

  /* Get the buffer associated with the text view widget. */
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
  /* Search from the start from buffer for text. */
  
	if(GTK_IS_TEXT_BUFFER (buffer))
{
	if(!GTK_IS_TEXT_VIEW (editor->textview)&&!GTK_IS_TEXT_TAG_TABLE (table1)){
	gtk_text_buffer_get_start_iter (buffer, &iter);
  	
	found = gtk_text_iter_forward_search (&iter, text, 0, &mstart, &mend, NULL);
	
	highlight_tag2 = gtk_text_buffer_create_tag(buffer, "blue_fg", 
							 "foreground", "blue", NULL);
	

	/*while(found == 1){
	
	found = gtk_text_iter_forward_search (&iter, text, 0, &mstart, &mend, NULL);

  if (found){
      /* If found, highlight the text. */
  //  gtk_text_buffer_select_range (buffer, &mstart, &mend);
	/*gtk_text_buffer_apply_tag_by_name( buffer,"blue_fg",&mstart,&mend );	
	iter = mend;
g_print("enter");
*/	
}


	else{
	

  /* Get the buffer associated with the text view widget. */
  //buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
  /* Search from the start from buffer for text. */
  gtk_text_buffer_get_start_iter (buffer, &iter);
  found = gtk_text_iter_forward_search (&iter, text, 0, &mstart, &mend, NULL);
	
	highlight_tag2 = gtk_text_buffer_create_tag(buffer, "blue_fg", 
							 "foreground", "blue", NULL);
	

	while(found == 1){
	
	found = gtk_text_iter_forward_search (&iter, text, 0, &mstart, &mend, NULL);

  if (found){
      /* If found, highlight the text. */
  //  gtk_text_buffer_select_range (buffer, &mstart, &mend);
	gtk_text_buffer_apply_tag_by_name( buffer,"blue_fg",&mstart,&mend );	
	iter = mend;
	g_print("enter");
	}

	}
	}
}
}


static void update_statusbar(GtkTextBuffer *buffer, GtkStatusbar  *statusbar);
static void mark_set_callback(GtkTextBuffer *buffer,const GtkTextIter *new_location, GtkTextMark *mark,gpointer data);
static void new_clicked (GtkToolButton*, TextEditor*);
static void open_clicked (GtkToolButton*, TextEditor*);
static void save_clicked (GtkToolButton*, TextEditor*);
static void cut_clicked (GtkToolButton*, TextEditor*);
static void copy_clicked (GtkToolButton*, TextEditor*);
static void paste_clicked (GtkToolButton*, TextEditor*);
static void find_clicked (GtkButton*, TextEditor*);
void undo_redo(GtkWidget *widget,  gpointer item);

gchar *
get_tip(gchar *text)
{
  gint i;
  gboolean found;

  found = FALSE;
  for (i = 0; i < NUM_TIPS; i++)
    {
      if (strcmp (text, tips[i][0]) == 0)
        {
          found = TRUE;
          break;
        }
    }
  if (!found)
    return NULL;

  return g_strdup (tips[i][1]);
}

GtkWidget *
tip_window_new (gchar *tip)
{
  GtkWidget *win;
  GtkWidget *label;
  GtkWidget *eb;
  GdkColormap *cmap;
  GdkColor color;
  PangoFontDescription *pfd;

  win = gtk_window_new (GTK_WINDOW_POPUP);
  gtk_container_set_border_width (GTK_CONTAINER (win), 0);

  eb = gtk_event_box_new ();
  gtk_container_set_border_width (GTK_CONTAINER (eb), 1);
  gtk_container_add (GTK_CONTAINER (win), eb);

  label = gtk_label_new (tip);  
  gtk_container_add (GTK_CONTAINER (eb), label);

  pfd = pango_font_description_from_string ("courier");
  gtk_widget_modify_font (label, pfd);
  
  cmap = gtk_widget_get_colormap (win);
  color.red = 0;
  color.green = 0;
  color.blue = 0;
  if (gdk_colormap_alloc_color (cmap, &color, FALSE, TRUE))
    gtk_widget_modify_bg (win, GTK_STATE_NORMAL, &color);
  else
    g_warning ("Color allocation failed!\n");

  cmap = gtk_widget_get_colormap (eb);
  color.red = 65535;
  color.green = 65535;
  color.blue = 45535;
  if (gdk_colormap_alloc_color (cmap, &color, FALSE, TRUE))
    gtk_widget_modify_bg (eb, GTK_STATE_NORMAL, &color);
  else
    g_warning ("Color allocation failed!\n");

  return win;
}


void
insert_open_brace(GtkWidget **tip_win, GtkWidget *text_view, GtkTextIter *arg1)
{
  GdkWindow *win;
  GtkTextIter start;
  GdkRectangle buf_loc;
  gint x, y;
  gint win_x, win_y;
  gchar *text;
  gchar *tip_text;
  
  /* Get the word at cursor. */
  start = *arg1;
  if (!gtk_text_iter_backward_word_start (&start))
    return;
  text = gtk_text_iter_get_text (&start, arg1);
  g_strstrip (text);

  /* Get the corresponding tooltip. */
  tip_text = get_tip(text);  
  if (tip_text == NULL)
    return;
  
  /* Calculate the tool tip window location. */
  gtk_text_view_get_iter_location (GTK_TEXT_VIEW (text_view), arg1, 
                                   &buf_loc);
 // g_printf ("Buffer: %d, %d\n", buf_loc.x, buf_loc.y);
  gtk_text_view_buffer_to_window_coords (GTK_TEXT_VIEW (text_view),
                                         GTK_TEXT_WINDOW_WIDGET,
                                         buf_loc.x, buf_loc.y,
                                         &win_x, &win_y);
  //g_printf ("Window: %d, %d\n", win_x, win_y);
  win = gtk_text_view_get_window (GTK_TEXT_VIEW (text_view), 
                                  GTK_TEXT_WINDOW_WIDGET);
  gdk_window_get_origin (win, &x, &y);

  /* Destroy any previous tool tip window. */
  if (*tip_win != NULL)
    gtk_widget_destroy (GTK_WIDGET (*tip_win));  
  
  /* Create a new tool tip window and place it at the caculated
     location. */
  *tip_win = tip_window_new (tip_text);
  g_free(tip_text);
  gtk_window_move (GTK_WINDOW (*tip_win), win_x + x, 
                   win_y + y + buf_loc.height);
  gtk_widget_show_all (*tip_win);
}

void
insert_close_brace (GtkWidget **tip_win)
{
  if (*tip_win != NULL)
    {
      gtk_widget_destroy (GTK_WIDGET (*tip_win));
      *tip_win = NULL;
    }
}

void
buffer_insert_text (GtkTextBuffer *textbuffer, GtkTextIter *arg1,
                    gchar *arg2, gint arg3, gpointer user_data)
{
  static GtkWidget *tip_win = NULL;

  if (strcmp (arg2, "(") == 0)
    {
      insert_open_brace(&tip_win, GTK_WIDGET (user_data), arg1);
    }

  if (strcmp (arg2, ")") == 0)
    {
      insert_close_brace(&tip_win);
    }
		
}

int main (int argc,
          char *argv[])
{
  GtkWidget *window, *scrolled_win, *vbox, *searchbar, *toolbar, *find, *menubar, *filemenu, *file, *quit, *statusbar ; 
  TextEditor *editor = g_slice_new (TextEditor);
  
  GtkToolItem *new;
  GtkToolItem *open;
  GtkToolItem *save;
  GtkToolItem *sep;
  GtkToolItem *exit;
  GtkToolItem *undo;
  GtkToolItem *redo;

  GtkTextBuffer *buffer;

	PangoFontDescription *pfd;
	
  gtk_init (&argc, &argv);
  
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Syntax Terror Editor");
  gtk_container_set_border_width (GTK_CONTAINER (window), 3);
  gtk_widget_set_size_request (window, 600, 500);
  
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);
	  
  editor->textview = gtk_text_view_new ();
  editor->search = gtk_entry_new ();
  find = gtk_button_new_from_stock (GTK_STOCK_FIND);
  gtk_entry_set_text (GTK_ENTRY (editor->search), "Search");
  
  g_signal_connect (G_OBJECT (find), "clicked",
                    G_CALLBACK (search_button_clicked), 
                    (gpointer) editor);

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->textview));
	g_signal_connect (G_OBJECT (buffer), "insert_text", 
                    G_CALLBACK (buffer_insert_text), editor->textview);
  
  toolbar = gtk_toolbar_new();
  gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
  gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);

  new = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new, -1);
  g_signal_connect (G_OBJECT (new), "clicked",
                    G_CALLBACK (new_clicked), 
                    (gpointer) editor);

  open = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open, -1);
  g_signal_connect (G_OBJECT (open), "clicked",
                    G_CALLBACK (open_clicked), 
                    (gpointer) editor);
  
  save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);
  g_signal_connect (G_OBJECT (save), "clicked",
                    G_CALLBACK (save_clicked), 
                    (gpointer) editor);

  sep = gtk_separator_tool_item_new();
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1); 

  undo = gtk_tool_button_new_from_stock(GTK_STOCK_UNDO);
  gtk_widget_set_name(GTK_WIDGET(undo), "undo");
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), undo, -1);
  
  
  redo = gtk_tool_button_new_from_stock(GTK_STOCK_REDO);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), redo, -1);
  
  g_signal_connect(G_OBJECT(redo), "clicked", 
        G_CALLBACK(undo_redo), undo);
  g_signal_connect(G_OBJECT(undo), "clicked", 
        G_CALLBACK(undo_redo), redo);
  
  exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
  gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);
  g_signal_connect (G_OBJECT (exit), "clicked",
                    G_CALLBACK (gtk_main_quit), 
                    (gpointer) editor);

  statusbar = gtk_statusbar_new();

  g_signal_connect(buffer, "changed",
        G_CALLBACK(update_statusbar), statusbar);

  g_signal_connect_object(buffer, "mark_set", 
        G_CALLBACK(mark_set_callback), statusbar, 0);

//	g_signal_connect(buffer, "changed",
//       G_CALLBACK(font_highlight), (gpointer)editor);


  menubar = gtk_menu_bar_new();
  filemenu = gtk_menu_new();
  
  file = gtk_menu_item_new_with_label("File");
  quit = gtk_menu_item_new_with_label("Quit");
  g_signal_connect (G_OBJECT (quit), "activate",
                    G_CALLBACK (gtk_main_quit), 
                    (gpointer) editor);
	
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);

 
  
  scrolled_win = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);
  gtk_container_add (GTK_CONTAINER (scrolled_win), editor->textview);

  searchbar = gtk_hbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (searchbar), editor->search, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (searchbar), find, FALSE, FALSE, 0);
 	

	pfd = pango_font_description_from_string ("courier");
  gtk_widget_modify_font (editor->textview, pfd);

  vbox = gtk_vbox_new (FALSE, 5);
  gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), toolbar, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), searchbar, FALSE, FALSE, 0);  
	gtk_box_pack_start (GTK_BOX (vbox), scrolled_win, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, FALSE, 0);

  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show_all (window);
  
  gtk_main();
  return 0;
}

/* Verify that the user want to create a new document. If so, delete
 * all of the text from the buffer. */
static void 
new_clicked (GtkToolButton *cut, 
             TextEditor *editor)
{
  GtkWidget *dialog;
  GtkTextBuffer *buffer;
  gint result;

  dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, 
                       GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, 
                       "All changes will be lost. Do you want to continue?");

  result = gtk_dialog_run (GTK_DIALOG (dialog));
  if (result == GTK_RESPONSE_YES)
  {
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
    gtk_text_buffer_set_text (buffer, "", -1);
  }
  
  gtk_widget_destroy (dialog);
}

/* Replace the content of the current buffer with the content of a file. */
static void 
open_clicked (GtkToolButton *cut, 
              TextEditor *editor)
{
  GtkWidget *dialog;
  gint result;
  GtkTextBuffer *buffer;
  gchar *content, *file;
  
  dialog = gtk_file_chooser_dialog_new ("Choose a file ..", NULL,
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        GTK_STOCK_OPEN, GTK_RESPONSE_APPLY,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        NULL);
  
  result = gtk_dialog_run (GTK_DIALOG (dialog));
  if (result == GTK_RESPONSE_APPLY)
  {
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
    file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

    g_file_get_contents (file, &content, NULL, NULL);
    gtk_text_buffer_set_text (buffer, content, -1);
    
    g_free (content);
    g_free (file);
  }

  gtk_widget_destroy (dialog);
}

/* Save the content of the current buffer to a file. */
static void 
save_clicked (GtkToolButton *cut, 
              TextEditor *editor)
{
  GtkWidget *dialog;
  gint result;
  GtkTextBuffer *buffer;
  gchar *content, *file;
  GtkTextIter start, end;
  
  dialog = gtk_file_chooser_dialog_new ("Save the file ..", NULL,
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        GTK_STOCK_SAVE, GTK_RESPONSE_APPLY,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        NULL);
  
  result = gtk_dialog_run (GTK_DIALOG (dialog));
  if (result == GTK_RESPONSE_APPLY)
  {
    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
    file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      
    gtk_text_buffer_get_bounds (buffer, &start, &end);
    content = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
      
    g_file_set_contents (file, content, -1, NULL);
    g_free (content);
    g_free (file);
  }

  gtk_widget_destroy (dialog);
}

/* Copy the selection to the clipboard and remove it from the buffer. */
static void 
cut_clicked (GtkToolButton *cut, 
             TextEditor *editor)
{
  GtkClipboard *clipboard;
  GtkTextBuffer *buffer;
  
  clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
  gtk_text_buffer_cut_clipboard (buffer, clipboard, TRUE);
}

/* Copy the selection to the clipboard. */
static void 
copy_clicked (GtkToolButton *copy, 
              TextEditor *editor)
{
  GtkClipboard *clipboard;
  GtkTextBuffer *buffer;
  
  clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
  gtk_text_buffer_copy_clipboard (buffer, clipboard);
}

/* Delete any selected text and insert the clipboard content into
 * the document. */
static void 
paste_clicked (GtkToolButton *paste, 
               TextEditor *editor)
{
  GtkClipboard *clipboard;
  GtkTextBuffer *buffer;
  
  clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
  gtk_text_buffer_paste_clipboard (buffer, clipboard, NULL, TRUE);
}

/* Search for a text string from the current cursor position if there is no
 * selected text, or one character after the cursor if there is. */
static void 
find_clicked (GtkButton *cut, 
              TextEditor *editor)
{
  const gchar *find;
  gchar *output;
  GtkWidget *dialog;
  GtkTextBuffer *buffer;
  GtkTextIter start, begin, end;
  gboolean success;
  gint i = 0;
  find = gtk_entry_get_text (GTK_ENTRY (editor->search));
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (editor->textview));
  gtk_text_buffer_get_start_iter (buffer, &start);
  success = gtk_text_iter_forward_search (&start, (gchar*) find, 0, &begin, &end, NULL);
  while (success)
  {
  gtk_text_iter_forward_char (&start);
  success = gtk_text_iter_forward_search (&start, (gchar*) find, 0,&begin, &end, NULL);
  start = begin;
  i++;
  }
  output = g_strdup_printf ("The string '%s' was found %i times!", find, i);
  dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO,
  GTK_BUTTONS_OK, output, NULL);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
  g_free (output);
}

void undo_redo(GtkWidget *widget,  gpointer item) 
{
  static int count = 2;
  const char *name = gtk_widget_get_name(widget);

  if ( strcmp(name, "undo") ) {
    count++;
  } else {
    count--;
  }
 
  if (count < 0) {
     gtk_widget_set_sensitive(widget, FALSE);
     gtk_widget_set_sensitive(item, TRUE);
  } 

  if (count > 5) {
     gtk_widget_set_sensitive(widget, FALSE);
     gtk_widget_set_sensitive(item, TRUE);
  }
}

static void update_statusbar(GtkTextBuffer *buffer,
    GtkStatusbar  *statusbar)
{
  gchar *msg;
  gint row, col;
  GtkTextIter iter;

  gtk_statusbar_pop(statusbar, 0); 

  gtk_text_buffer_get_iter_at_mark(buffer,
      &iter, gtk_text_buffer_get_insert(buffer));

  row = gtk_text_iter_get_line(&iter);
  col = gtk_text_iter_get_line_offset(&iter);

  msg = g_strdup_printf("Col %d Ln %d", col+1, row+1);

  gtk_statusbar_push(statusbar, 0, msg);

  g_free(msg);
}

static void
mark_set_callback(GtkTextBuffer *buffer,
    const GtkTextIter *new_location, GtkTextMark *mark,
    gpointer data)
{
  update_statusbar(buffer, GTK_STATUSBAR(data));
}
