<?php
/**
 * @file
 * A basic template for fileshare entities
 *
 * Available variables:
 * - $content: An array of comment items. Use render($content) to print them
 *   all, or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $title: The subject of the fileshare
 * - $url: The standard URL for viewing a fileshare entity
 * - $page: TRUE if this is the main view page $url points too.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. By default the following classes are available, where
 *   the parts enclosed by {} are replaced by the appropriate values:
 *   - entity-profile
 *   - fileshare-{TYPE}
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess()
 * @see template_preprocess_entity()
 * @see template_process()
 */
$sender = user_load($fileshare->sender);
$recipient = user_load($fileshare->recipient);
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="content"<?php print $content_attributes; ?>>
    <div class="fileshare-header-buttons clearfix">
      <a href="/messages/inbox" title="<?php print t('Back to inbox'); ?>" alt="<?php print t('Back to inbox'); ?>" 
        class="button"><?php print t('Back to inbox'); ?></a>
      <a href="/messages/compose" title="<?php print t('Compose a new message'); ?>" alt="<?php print t('Compose a new message'); ?>" 
        class="button"><?php print t('Compose'); ?></a>
    </div>
    <div class="fileshare-header clearfix">
      <table>
        <tr>
          <th><?php print t('Date sent'); ?></th>
          <td><?php print format_date($fileshare->date_sent); ?></td>
        </tr>
        <?php if ($fileshare->status == 1): ?>
          <tr>
            <th><?php print t('Date read'); ?></th>
            <td><?php print format_date($fileshare->date_read); ?></td>
          </tr>
        <?php endif; ?>
        <tr>
          <th><?php print t('Sender'); ?></th>
          <td><a href="/messages/filter/<?php print $sender->uid; ?>"><?php print $sender->name; ?></a></td>
        </tr>
        <tr>
          <th><?php print t('Recipient'); ?></th>
          <td><a href="/messages/filter/<?php print $recipient->uid; ?>"><?php print $recipient->name; ?></a></td>
        </tr>
        <?php if (!empty($fileshare->data['cc'])): ?>
          <tr>
            <th><?php print t('CC'); ?></th>
            <td><?php print check_plain(implode(', ', $fileshare->data['cc'])); ?></td>
          </tr>
        <?php endif; ?>
        <tr>
          <th><?php print t('Subject'); ?></th>
          <td><?php print check_plain($fileshare->subject); ?></td>
        </tr>
      </table>
    </div>
    <?php print render($content); ?>
    <?php if (fileshare_access('reply', $fileshare)): ?>
      <div class="fileshare-footer clearfix">
        <div class="actions">
          <a href="/messages/message/<?php print $fileshare->fileshare_id; ?>/reply" class="button"
            alt="<?php print t('Reply to this message'); ?>" title="<?php print t('Reply to this message'); ?>">
            <?php print t('Reply'); ?>
          </a>
        </div>
      </div>
    <?php endif; ?>
  </div>
</div>
