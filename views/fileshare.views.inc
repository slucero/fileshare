<?php
/**
 * @file
 * Views data definitions.
 */

/**
 * Implements hook_views_data_alter().
 */
function fileshare_views_data_alter(&$data) {
  $data['fileshare']['link_fileshare'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the fileshare.'),
      'handler' => 'fileshare_handler_link_field',
    ),
  );
  $data['fileshare']['subject'] = array(
    'field' => array(
      'title' => t('Subject'),
      'help' => t('The subject of the fileshare message.'),
      'handler' => 'fileshare_handler_subject_field',
    ),
  );
  $data['fileshare']['status'] = array(
    'field' => array(
      'title' => t('Status'),
      'help' => t('The read status of the fileshare message.'),
      'handler' => 'fileshare_handler_status_field',
    ),
  );
  $data['fileshare']['delete_fileshare'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the fileshare.'),
      'handler' => 'fileshare_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows fileshares/fileshare/%fileshare_id/op
  $data['fileshare']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this fileshare.'),
      'handler' => 'fileshare_handler_fileshare_operations_field',
    ),
  );
  $data['fileshare']['parties_involved'] = array(
    'help' => t('All individuals involved in a fileshare message.'),
    'argument' => array(
      'title' => t('Parties Involved'),
      'name table' => 'users',
      'name field' => 'name',
      'numeric' => TRUE,
      'handler' => 'fileshare_handler_parties_involved_argument',
    ),
  );
  $data['fileshare']['user_involved'] = array(
    'help' => t('All individuals involved in a fileshare message.'),
    'argument' => array(
      'title' => t('User Involved'),
      'name table' => 'users',
      'name field' => 'name',
      'numeric' => TRUE,
      'handler' => 'fileshare_handler_user_involved_argument',
    ),
  );
  $data['fileshare']['inbox'] = array(
    'help' => t('All messages that should be displayed in a user\'s inbox.'),
    'argument' => array(
      'title' => t('Inbox'),
      'numeric' => TRUE,
      'handler' => 'fileshare_handler_inbox_argument',
    ),
  );
}
