<?php

/**
 * @file
 * Fileshare editing UI
 *
 */

/**
 * Fileshare UI controller
 */
class FileshareUIController extends EntityDefaultUIController {

  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items['messages/compose'] = array(
      'title' => 'Compose a new message',
      'page callback' => 'fileshare_add_page',
      'access callback' => 'fileshare_access',
      'access arguments' => array('create'),
      'type' => MENU_NORMAL_ITEM,
      'file' => 'fileshare.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    // Add menu items for each different type of fileshare
    foreach (fileshare_get_types() as $type) {
      $items['messages/compose/' . $type->type] = array(
        'title' => 'Compose new ' . $type->label,
        'page callback' => 'fileshare_form_wrapper',
        'page arguments' => array(fileshare_create(array('type' => $type->type)), 'create'),
        'access callback' => 'fileshare_access',
        'access arguments' => array('create'),
        'file' => 'fileshare.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module']),
      );
    }

    // Menu items for individual messages
    $items['messages/message/' . $wildcard] = array(
      'title' => 'View',
      'page callback' => 'fileshare_page_view',
      'page arguments' => array(2),
      'access callback' => 'fileshare_access',
      'access arguments' => array('view', 2),
      'type' => MENU_NORMAL_ITEM,
      'file' => 'fileshare.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );
    $items['messages/message/' . $wildcard . '/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => -10,
    );
    $items['messages/message/' . $wildcard . '/reply'] = array(
      'title' => 'Reply',
      'page callback' => 'fileshare_reply_form_wrapper',
      'page arguments' => array(2),
      'access callback' => 'fileshare_access',
      'access arguments' => array('reply', 2),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'fileshare.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );

    return $items;
  }

  /**
   * Create the markup for the add Fileshare Entities page within the class
   * so it may be easily overwritten/extended.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    // Bypass type select if only one type exists
    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('fileshare_add_list', array('content' => $content));
  }
}

/**
 * Form callback wrapper: create or edit a fileshare.
 *
 * @param $fileshare
 *   The fileshare object being edited by this form.
 *
 * @see fileshare_edit_form()
 */
function fileshare_form_wrapper($fileshare = NULL, $op = 'edit') {
  // Add the breadcrumb for the fileshare's location.
  //fileshare_set_breadcrumb();
  return drupal_get_form('fileshare_edit_form', $fileshare, $op);
}


/**
 * Form callback wrapper: delete a fileshare.
 *
 * @param $fileshare
 *   The fileshare object being edited by this form.
 *
 * @see fileshare_edit_form()
 */
function fileshare_delete_form_wrapper($fileshare) {
  // Add the breadcrumb for the form's location.
  //fileshare_set_breadcrumb();
  return drupal_get_form('fileshare_delete_form', $fileshare);
}

/**
 * Form callback wrapper: reply to a fileshare.
 *
 * @param $fileshare
 *   The fileshare object being edited by this form.
 *
 * @see fileshare_edit_form()
 */
function fileshare_reply_form_wrapper($fileshare) {
  // Add the breadcrumb for the form's location.
  //fileshare_set_breadcrumb();

  // Set the page title
  drupal_set_title(t('Reply to message: %subject', array('%subject' => $fileshare->subject)), PASS_THROUGH);

  return drupal_get_form('fileshare_reply_form', $fileshare);
}

/**
 * Form callback function to reply to a fileshare.
 *
 * @todo remove hardcoded links in submission buttons
 */
function fileshare_reply_form($form, &$form_state, $fileshare, $op = 'reply') {
  global $user;

  $recipient = user_load($fileshare->sender);
  $reply = fileshare_create(array(
    'sender' => $user->uid,
    'recipient' => $recipient->uid,
    'status' => 0,
    'subject' => 'Re: ' . $fileshare->subject, 
    'type' => $fileshare->type,
		'data' => array(
			'cc' => $fileshare->data['cc'],
		),
  ));

  // Set the sender ID for new fileshares
  if ($op == 'create') {
    $form['sender'] = array(
      '#type' => 'value',
      '#value' => $reply->sender,
    );
  }
  
  // Set reply values
  $form['recipient'] = array(
    '#type' => 'value',
    '#value' => $reply->recipient,
  );
  $form['subject'] = array(
    '#type' => 'value',
    '#value' => $reply->subject,
  );

  // Set display items for reply values
  $form['recipient_display'] = array(
    '#title' => t('To'),
    '#type' => 'item',
    '#markup' => $recipient->name,
  );
  $form['subject_display'] = array(
    '#title' => t('Subject'),
    '#type' => 'item',
    '#markup' => check_plain($reply->subject),
  );

  $form['carbon'] = array(
    '#title' => t('CC'),
    '#type' => 'textfield',
    '#description' => t('Enter a comma-separated list of email addresses to receive notifications.<br/>
      <strong>Note:</strong> The recipient must have an account to view any messages sent.'),
		'#default_value' => $reply->data['cc'],
  );

  // Add field related form elements
  $form_state['fileshare'] = $reply;
  field_attach_form('fileshare', $reply, $form, $form_state);

  $submit = array();

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send reply'),
    '#submit' => $submit + array('fileshare_reply_form_submit'),
  );

  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), 'messages'),
  );

  $form['previous_message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Previous message'),
    '#weight' => 100,
  );

  $form['previous_message']['preview'] = field_attach_view('fileshare', $fileshare, 'full');

  $form['#validate'][] = 'fileshare_reply_form_validate';

  return $form;
}

/**
 * Form API validate callback for the fileshare reply form
 * @see fileshare_form().
 */
function fileshare_reply_form_validate(&$form, &$form_state) {
  $fileshare = $form_state['fileshare'];
  
  // Validate the Carbon Copy field
  if (!empty($form_state['values']['carbon'])) {
    $cc_array = explode(',', $form_state['values']['carbon']);
    foreach ($cc_array as $email) {
      if (!valid_email_address(trim($email))) {
        form_set_error('carbon', t('%email is not a valid email address.', array('%email' => $email)));
      }
    }
  }

  // Notify field widgets to validate their data.
  field_attach_form_validate('fileshare', $fileshare, $form, $form_state);
}

/**
 * Form submit callback function for the fileshare reply form.
 *
 * @see fileshare_reply_form().
 *
 * @todo remove hard-coded path
 */
function fileshare_reply_form_submit(&$form, &$form_state) {
  $fileshare = entity_ui_controller('fileshare')->entityFormSubmitBuildEntity($form, $form_state);

  // Apply the new send time
  $fileshare->date_sent = time();

  // Apply new read status
  $fileshare->status = 0;

  $cc = explode(',', $form_state['values']['carbon']);

  $copied = array();
  // Trim the whitespace from all entered email addresses
  foreach ($cc as $index => $email) {
    $email = trim($email);
    if (!empty($email)) {
      $copied[$index] = $email;
    }
  }

  $fileshare->data['cc'] = $copied;

  fileshare_save($fileshare);
  $form_state['redirect'] = 'messages/inbox';
}

/**
 * Form callback function to create or edit a fileshare.
 *
 * @todo remove hardcoded links in submission buttons
 */
function fileshare_edit_form($form, &$form_state, $fileshare, $op = 'edit') {
  global $user;

  // Set the sender ID for new fileshares
  if ($op == 'create') {
    $form['sender'] = array(
      '#type' => 'value',
      '#value' => $user->uid,
    );
  }

  // Build the recipient list
  $recipients = $suggested_users = fileshare_get_suggested_users();
  if (empty($suggested_users)) {
    drupal_set_message(t('Permissions have not been set to allow any suggested users.'));
  }
  
  // Create default field elements
  /*
  $form['recipient'] = array(
    '#title' => t('To'),
    '#type' => 'textfield',
    '#description' => t('Enter the name of the recipient.'),
    '#autocomplete_path' => 'user/autocomplete',
    '#maxlength' => 60,
    //'#value_callback' => 'fileshare_field_recipient_value',
  );
   */

  $form['recipient'] = array(
    '#title' => t('To'),
    '#type' => 'select',
    '#description' => t('Select the person this message should be sent to.'),
    '#options' => $recipients,
  );
  $form['carbon'] = array(
    '#title' => t('CC'),
    '#type' => 'textfield',
    '#description' => t('Enter a comma-separated list of email addresses to receive notifications.<br/>
      <strong>Note:</strong> The recipient must have an account to view any messages sent.'),
  );
  $form['subject'] = array(
    '#title' => t('Subject'),
    '#type' => 'textfield',
    '#default_value' => isset($fileshare->subject) ? $fileshare->subject : '',
    '#description' => t('The subject for this fileshare message.'),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#weight' => -5,
  );

  // Add field related form elements
  $form_state['fileshare'] = $fileshare;
  field_attach_form('fileshare', $fileshare, $form, $form_state);

  $submit = array();

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send Message'),
    '#submit' => $submit + array('fileshare_edit_form_submit'),
  );

  if (!empty($fileshare->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete fileshare'),
      '#suffix' => l(t('Cancel'), 'admin/content/fileshares'),
      '#submit' => $submit + array('fileshare_edit_form_submit_delete'),
      '#weight' => 45,
    );
  }

  $form['#validate'][] = 'fileshare_edit_form_validate';
  $form['#entity_builders'][] = 'fileshare_edit_form_build_entity';

  return $form;
}

/**
 * Form API validate callback for the fileshare form
 *
 * @see fileshare_edit_form().
 */
function fileshare_edit_form_validate(&$form, &$form_state) {
  $fileshare = $form_state['fileshare'];

  // Validate the recipient's access to the fileshare system
  if ($recipient = user_load($form_state['values']['recipient'])) {
    if (!user_access('use fileshare system', $recipient)) {
      form_set_error('recipient', t('@recipient cannot receive messages.', array('@recipient' => $recipient->name)));
    }
  } 
  else {
    form_set_error('recipient', t('Please enter a valid user to send the message to.'));
  }

  // Validate the Carbon Copy field
  if (!empty($form_state['values']['carbon'])) {
    $cc_array = explode(',', $form_state['values']['carbon']);
    foreach ($cc_array as $email) {
      if (!valid_email_address(trim($email))) {
        form_set_error('carbon', t('%email is not a valid email address.', array('%email' => $email)));
      }
    }
  }
  
  // Notify field widgets to validate their data.
  field_attach_form_validate('fileshare', $fileshare, $form, $form_state);
}

/**
 * Form submit callback function for the fileshare form.
 *
 * @see fileshare_form().
 *
 * @todo remove hard-coded path
 */
function fileshare_edit_form_submit(&$form, &$form_state) {
  $fileshare = entity_ui_controller('fileshare')->entityFormSubmitBuildEntity($form, $form_state);
  $fileshare->recipient = $form_state['values']['recipient'];

  if (!isset($fileshare->date_sent)) {
    $fileshare->date_sent = time();
  }

  $fileshare->status = 0;

  $cc = explode(',', $form_state['values']['carbon']);

  $copied = array();
  // Trim the whitespace from all entered email addresses
  foreach ($cc as $index => $email) {
    $email = trim($email);
    if (!empty($email)) {
      $copied[$index] = $email;
    }
  }

  $fileshare->data['cc'] = $copied;

  fileshare_save($fileshare);

  drupal_set_message(t('Your message has been sent.'));

  $form_state['redirect'] = 'messages/inbox';
}

/**
 * Entity builder function to save recipient values.
 */
function fileshare_edit_form_build_entity($entity_type, &$entity, $form, &$form_state) {
  $recipient = user_load_by_name($form_state['values']['recipient']);

  // Suppressing error message to avoid errors when creating new entities
  @$entity->recipient = $recipient->uid;
}

/**
 *
 */
function fileshare_field_recipient_value($element, $input = FALSE, $form_state) {
  if ($input) {
    $recipient = user_load_by_name($input);

    return $recipient->uid;
  }
}

/**
 * Page to add Fileshare Entities.
 *
 * @todo Pass this through a proper theme function
 */
function fileshare_add_page() {
  $controller = entity_ui_controller('fileshare');
  return $controller->addPage();
}


/**
 * Displays the list of available fileshare types for fileshare creation.
 *
 * @ingroup themeable
 */
function theme_fileshare_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="fileshare-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer fileshare types')) {
      $output = '<p>' . t('Fileshare Entities cannot be added because you have not created any fileshare types yet. Go to the <a href="@create-fileshare-type">fileshare type creation page</a> to add a new fileshare type.', array('@create-fileshare-type' => url('admin/structure/fileshare_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No fileshare types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}
