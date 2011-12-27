<?php
/**
 * Dec 20, 2011
 * @file og_simplenews.pages.inc
 * @author blazey http://drupal.org/user/353861
 *
 * @todo Reexport newsletter_issues view.
 */

/**
 * Current group's newsletter manager form.
 *
 * @return array
 */
function og_simplenews_manager_form(&$form_state) {
  $form_state['node'] = og_get_group_context();
  $newsletter_terms = og_simplenews_get_group_newsletter_terms($form_state['node']);

  $form['newsletters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Newsletters'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
  );

  $form['newsletters']['new'] = array(
    '#type' => 'textfield',
    '#title' => t('Add newsletter mailing list'),
    '#default_value' => '',
  );
  $form['newsletters']['new_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('og_simplenews_manager_add_newsletter'),
  );

  foreach ($newsletter_terms as $term) {
    $form['newsletters'][$term->tid] = array(
      '#type' => 'fieldset',
      '#title' => $term->name,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['newsletters'][$term->tid]['emails'] = array(
      '#title' => t('Addresses'),
      '#type' => 'textarea',
      '#default_value' => implode("\n", og_simplenews_get_subscriptions($term->tid)),
      '#description' => t('Enter list of emails, each in a new line.'),
    );
  }

  $form['newsletters']['emails_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save email addresses'),
    '#submit' => array('og_simplenews_manager_save_emails'),
  );

  $form['add_new_link'] = array(
    '#value' => l(t('Add new issue'), 'node/add/simplenews'),
  );

  $form['view'] = array(
    '#value' => views_embed_view('newsletter_issues', 'default', implode('+', array_keys($newsletter_terms))),
  );

  return $form;
}

/**
 * Newsletter manager form submit handler. Saves emails as subscriptions.
 */
function og_simplenews_manager_save_emails($form, &$form_state) {
  foreach ($form_state['values']['newsletters'] as $tid => $values) {
    if ($tid && is_numeric($tid)) {
      og_simplenews_save_subscriptions($tid, $values['emails']);
    }
  }
}

/**
 * Newsletter manager form submit handler. Adds new newsletter.
 */
function og_simplenews_manager_add_newsletter($form, &$form_state) {
  $name = $form_state['values']['newsletters']['new'];
  if (!empty($name)) {
    og_simplenews_new_group_newsletter($form_state['node'], $name);
  }
}

/**
 * Get emails subscribed to target newsletter.
 *
 * @param int $tid
 * @return array
 */
function og_simplenews_get_subscriptions($tid) {
  $emails = array();

  $result = db_query('
    SELECT ss.mail
    FROM {simplenews_subscriptions} ss
    INNER JOIN {simplenews_snid_tid} sn
    ON ss.snid = sn.snid
    WHERE sn.tid = %d', $tid);

  while ($row = db_fetch_array($result)) {
    $emails[] = $row['mail'];
  }

  return $emails;
}

/**
 * Subscribes emails to target newsletter
 *
 * @param int $tid
 * @param string $emails
 */
function og_simplenews_save_subscriptions($tid, $emails) {
  // First, delete all subscriptions.
  db_query('DELETE FROM {simplenews_snid_tid} WHERE tid = %d', $tid);

  // And now add new. Simplenews sucks.
  require_once drupal_get_path('module', 'simplenews') . '/includes/simplenews.admin.inc';
  $form_state['values'] = array(
    'emails' => $emails,
    'newsletters' => array(
      $tid => $tid,
    ),
  );
  drupal_execute('simplenews_subscription_list_add', $form_state);
}